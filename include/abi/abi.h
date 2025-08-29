#pragma once
#include <array>
#include <vector>
#include <tuple>
#include <string>
#include <string_view>
#include <type_traits>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <utility>
#include <cassert>

#include <boost/multiprecision/cpp_int.hpp>

namespace abi {

// ----------------- minimal Span<T> for C++17 -----------------
template<class T>
struct Span {
  const T* data_{nullptr};
  size_t   size_{0};
  Span() = default;
  Span(const T* p, size_t n): data_(p), size_(n) {}
  const T* data() const { return data_; }
  size_t size() const { return size_; }
  const T& operator[](size_t i) const { return data_[i]; }
  const T* begin() const { return data_; }
  const T* end()   const { return data_ + size_; }
};

using BytesSpan = Span<const uint8_t>;

// ----------------- error -----------------
struct Error {
  std::string message, path;
  size_t offset{};
};

// ----------------- schema descriptors -----------------
template<int N> struct uint_t {};
template<int N> struct  int_t {};
struct bool_t {};
struct address20 {};
struct bytes {};        // dynamic
struct string_t {};     // dynamic

template<class T, size_t N> struct static_array { using elem = T; static constexpr size_t n = N; };
template<class T>            struct dyn_array    { using elem = T; };
template<class... Ts>        struct tuple       {};

template<class Schema> struct traits;
// ----------------- default value mapping -----------------
template<class T> struct value_of;
template<int N> struct value_of<uint_t<N>> { using type = boost::multiprecision::cpp_int; };
template<int N> struct value_of< int_t<N>> { using type = boost::multiprecision::cpp_int; };
template<> struct value_of<bool_t>    { using type = bool; };
template<> struct value_of<address20> { using type = std::array<uint8_t,20>; };
template<> struct value_of<bytes>     { using type = std::vector<uint8_t>; };
template<> struct value_of<string_t>  { using type = std::string; };
template<class T, size_t N> struct value_of<static_array<T,N>> { using type = std::array<typename value_of<T>::type, N>; };
template<class T>           struct value_of<dyn_array<T>>      { using type = std::vector<typename value_of<T>::type>; };
template<class... Ts>       struct value_of<tuple<Ts...>>      { using type = std::tuple<typename value_of<Ts>::type...>; };

// ----------------- helpers -----------------
inline constexpr size_t pad32(size_t n){ return (n + 31) & ~size_t(31); }

inline void write_u256_imm(uint8_t* out, uint64_t v){
  std::memset(out, 0, 32);
  for(int i=0;i<8;++i){ out[31-i]=uint8_t(v&0xFF); v>>=8; }
}
inline void write_u256_big(uint8_t* out, const boost::multiprecision::cpp_int& x){
  using boost::multiprecision::cpp_int;
  cpp_int v = x;
  for(int i=31;i>=0;--i){ out[i] = static_cast<uint8_t>((v & 0xFF).convert_to<unsigned>()); v >>= 8; }
}
inline boost::multiprecision::cpp_int read_u256_big(const uint8_t* in){
  using boost::multiprecision::cpp_int;
  cpp_int x=0; for(int i=0;i<32;++i){ x <<= 8; x += in[i]; } return x;
}
template<int N>
inline boost::multiprecision::cpp_int sign_extend(const boost::multiprecision::cpp_int& x){
  using boost::multiprecision::cpp_int;
  static_assert(N>=1 && N<=256, "int bits");
  const cpp_int one=1;
  const cpp_int mask = (one<<N) - 1;
  cpp_int v = x & mask;
  const bool neg = ((v >> (N-1)) & 1) != 0;
  if(neg){ return v - (one<<N); }
  return v;
}
inline void write_bytes_padded(uint8_t* out, BytesSpan b){
  if (b.size()) std::memcpy(out, b.data(), b.size());
  size_t pad = pad32(b.size()) - b.size();
  if (pad) std::memset(out + b.size(), 0, pad);
}
inline BytesSpan as_bytes_span(const std::vector<uint8_t>& v){ return {v.data(), v.size()}; }
inline BytesSpan as_bytes_span(const std::string& s){ return {reinterpret_cast<const uint8_t*>(s.data()), s.size()}; }

template<class S> struct needs_top_ptr : std::false_type {};
template<> struct needs_top_ptr<bytes> : std::true_type {};
template<> struct needs_top_ptr<string_t> : std::true_type {};
template<class T> struct needs_top_ptr<dyn_array<T>> : std::true_type {};
template<class... Ts>
struct needs_top_ptr<abi::tuple<Ts...>>
  : std::bool_constant<abi::traits<abi::tuple<Ts...>>::is_dynamic> {};
template<class T, size_t N>
struct needs_top_ptr<static_array<T,N>> : std::bool_constant<traits<T>::is_dynamic> {};

// ----------------- traits primary -----------------
template<class Schema> struct traits;

// bool
template<> struct traits<bool_t>{
  static constexpr bool is_dynamic=false; static constexpr size_t head_words=1;
  template<class V> static size_t tail_size(const V&){ return 0; }
  template<class V> static void encode_head(uint8_t* out32, size_t, const V& v, size_t){ std::memset(out32,0,32); out32[31] = v?1:0; }
  template<class V> static void encode_tail(uint8_t*, size_t, const V&){}
  static bool decode(BytesSpan in, bool& out, Error* e=nullptr){
    if(in.size()<32){ if(e) e->message="bool: short"; return false; } out = (in.data()[31]!=0); return true;
  }
};
// address
template<> struct traits<address20>{
  static constexpr bool is_dynamic=false; static constexpr size_t head_words=1;
  template<class V> static size_t tail_size(const V&){ return 0; }
  template<class V> static void encode_head(uint8_t* out32, size_t, const V& v, size_t){
    std::memset(out32,0,12);
    if constexpr (std::is_same<V,std::array<uint8_t,20>>::value) {
      std::memcpy(out32+12, v.data(), 20);
    } else {
      auto s = as_bytes_span(v); assert(s.size()==20); std::memcpy(out32+12, s.data(), 20);
    }
  }
  template<class V> static void encode_tail(uint8_t*, size_t, const V&){}
  static bool decode(BytesSpan in, std::array<uint8_t,20>& out, Error* e=nullptr){
    if(in.size()<32){ if(e) e->message="address: short"; return false; }
    std::memcpy(out.data(), in.data()+12, 20); return true;
  }
};
// uintN
template<int N> struct traits<uint_t<N>>{
  static_assert(N>=8&&N<=256,"uintN");
  static constexpr bool is_dynamic=false; static constexpr size_t head_words=1;
  template<class V> static size_t tail_size(const V&){ return 0; }
  template<class V> static void encode_head(uint8_t* out32, size_t, const V& v, size_t){
    using boost::multiprecision::cpp_int;
    if constexpr (std::is_integral<V>::value) write_u256_imm(out32, static_cast<uint64_t>(v));
    else write_u256_big(out32, static_cast<cpp_int>(v));
  }
  template<class V> static void encode_tail(uint8_t*, size_t, const V&){}
  static bool decode(BytesSpan in, typename value_of<uint_t<N>>::type& out, Error* e=nullptr){
    if(in.size()<32){ if(e) e->message="uintN: short"; return false; }
    out = read_u256_big(in.data()); return true;
  }
};
// intN
template<int N> struct traits<int_t<N>>{
  static_assert(N>=8&&N<=256,"intN");
  static constexpr bool is_dynamic=false; static constexpr size_t head_words=1;
  template<class V> static size_t tail_size(const V&){ return 0; }
  template<class V> static void encode_head(uint8_t* out32, size_t, const V& v, size_t){
    using boost::multiprecision::cpp_int; write_u256_big(out32, static_cast<cpp_int>(v));
  }
  template<class V> static void encode_tail(uint8_t*, size_t, const V&){}
  static bool decode(BytesSpan in, typename value_of<int_t<N>>::type& out, Error* e=nullptr){
    if(in.size()<32){ if(e) e->message="intN: short"; return false; }
    out = sign_extend<N>(read_u256_big(in.data())); return true;
  }
};
// bytes (dynamic)
template<> struct traits<bytes>{
  static constexpr bool is_dynamic=true; static constexpr size_t head_words=1;
  template<class V> static size_t tail_size(const V& v){ auto s = as_bytes_span(v); return 32 + pad32(s.size()); }
  template<class V> static void encode_head(uint8_t* out32, size_t, const V&, size_t base){ write_u256_imm(out32, base); }
  template<class V> static void encode_tail(uint8_t* out, size_t base, const V& v){
    auto s = as_bytes_span(v); write_u256_imm(out+base, s.size()); write_bytes_padded(out+base+32, s);
  }
  static bool decode(BytesSpan in, std::vector<uint8_t>& out, Error* e=nullptr){
    if(in.size()<32){ if(e) e->message="bytes: short"; return false; }
    auto len = static_cast<size_t>(read_u256_big(in.data()).convert_to<unsigned long long>());
    if(in.size() < 32 + len){ if(e) e->message="bytes: len out of range"; return false; }
    out.resize(len); if(len) std::memcpy(out.data(), in.data()+32, len); return true;
  }
};
// string (dynamic)
template<> struct traits<string_t>{
  static constexpr bool is_dynamic=true; static constexpr size_t head_words=1;
  static size_t tail_size(const std::string& v){ return 32 + pad32(v.size()); }
  static void encode_head(uint8_t* out32, size_t, const std::string&, size_t base){ write_u256_imm(out32, base); }
  static void encode_tail(uint8_t* out, size_t base, const std::string& v){
    write_u256_imm(out+base, v.size());
    if(!v.empty()) std::memcpy(out+base+32, v.data(), v.size());
    size_t pad = pad32(v.size()) - v.size(); if(pad) std::memset(out+base+32+v.size(), 0, pad);
  }
  static bool decode(BytesSpan in, std::string& out, Error* e=nullptr){
    if(in.size()<32){ if(e) e->message="string: short"; return false; }
    auto len = static_cast<size_t>(read_u256_big(in.data()).convert_to<unsigned long long>());
    if(in.size() < 32 + len){ if(e) e->message="string: len out of range"; return false; }
    out.assign(reinterpret_cast<const char*>(in.data()+32), len); return true;
  }
};
// static_array
template<class T, size_t N>
struct traits<static_array<T,N>>{
  static constexpr bool elem_dyn = traits<T>::is_dynamic;
  static constexpr bool is_dynamic = elem_dyn; // ABI: static array of dynamic is dynamic overall
  static constexpr size_t head_words = elem_dyn ? 1 : (traits<T>::head_words * N);

  template<class V> static size_t tail_size(const V& v){
    if(!elem_dyn) return 0;
    size_t bytes = 32 * N; // offsets
    for(size_t i=0;i<N;++i) bytes += traits<T>::tail_size(v[i]);
    return bytes;
  }
  template<class V> static void encode_head(uint8_t* out32, size_t /*head_idx*/, const V& v, size_t base){
    if(!elem_dyn){
      size_t w=0;
      for(size_t i=0;i<N;++i){
        traits<T>::encode_head(out32 + 32*w, 0, v[i], 0);
        w += traits<T>::head_words;
      }
    }else{
      write_u256_imm(out32, base);
    }
  }
  template<class V> static void encode_tail(uint8_t* out, size_t base, const V& v){
    if(!elem_dyn) return;
    size_t cursor = base; size_t running = 32*N;
    for(size_t i=0;i<N;++i){
      write_u256_imm(out + cursor + 32*i, running);
      traits<T>::encode_tail(out, base + running, v[i]);
      running += traits<T>::tail_size(v[i]);
    }
  }

  static bool decode(BytesSpan in,
                     typename value_of<static_array<T,N>>::type& out,
                     Error* e=nullptr)
  {
    if constexpr (!elem_dyn) {
      const size_t need = 32 * traits<T>::head_words * N;
      if (in.size() < need) { if (e) e->message = "static_array: short"; return false; }

      size_t off = 0;
      for (size_t i = 0; i < N; ++i) {
        if (!traits<T>::decode(BytesSpan(in.data() + off, 32 * traits<T>::head_words), out[i], e)) return false;
        off += 32 * traits<T>::head_words;
      }
      return true;
    } else {
      // dynamic element: head is N offsets, each offset is RELATIVE TO THE START OF THIS ARRAY ENCODING
      const size_t head_bytes = 32 * N;
      if (in.size() < head_bytes) { if (e) e->message = "static_array(dyn): head short"; return false; }

      // collect offsets
      std::array<size_t, N + 1> offs{};
      for (size_t i = 0; i < N; ++i) {
        size_t oi = static_cast<size_t>(read_u256_big(in.data() + 32 * i).convert_to<unsigned long long>());
        // must point somewhere within the array slice
        if (oi > in.size()) { if (e) e->message = "static_array(dyn): bad off"; return false; }
        // sanity: usually at/after head
        if (oi < head_bytes) { /* allow or enforce; if you want strictness: if(e) e->message="static_array(dyn): off < head"; return false; */ }
        offs[i] = oi;
      }
      offs[N] = in.size(); // sentinel end

      // decode each element on a bounded slice [offs[i], offs[i+1])
      for (size_t i = 0; i < N; ++i) {
        size_t start = offs[i];
        size_t stop  = offs[i + 1];
        if (stop < start) { if (e) e->message = "static_array(dyn): off order"; return false; }
        size_t span = stop - start;
        if (!traits<T>::decode(BytesSpan(in.data() + start, span), out[i], e)) return false;
      }
      return true;
    }
  }
};
// dyn_array

template<class T>
struct traits<dyn_array<T>>{
  static constexpr bool elem_dyn = traits<T>::is_dynamic;
  static constexpr bool is_dynamic = true;
  static constexpr size_t head_words = 1;

  template<class V> static size_t tail_size(const V& v){
    size_t bytes = 32; // length
    if (elem_dyn) {
      bytes += 32 * v.size();
      for (const auto& e: v) bytes += traits<T>::tail_size(e);
    } else {
      bytes += 32 * traits<T>::head_words * v.size();
    }
    return bytes;
  }

  template<class V>
  static void encode_head(uint8_t* out32, size_t, const V&, size_t base){
    write_u256_imm(out32, base);
  }

  template<class V>
  static void encode_tail(uint8_t* out, size_t base, const V& v){
    write_u256_imm(out + base, v.size());
    size_t cursor = base + 32;

    if (elem_dyn) {
      size_t running = 32 * v.size();
      for (size_t i = 0; i < v.size(); ++i) {
        write_u256_imm(out + cursor + 32*i, running);
        traits<T>::encode_tail(out, base + 32 + running, v[i]);
        running += traits<T>::tail_size(v[i]);
      }
    } else {
      // ✅ FIX: use full stride for static elements
      const size_t stride = 32 * traits<T>::head_words;
      for (size_t i = 0; i < v.size(); ++i) {
        traits<T>::encode_head(out + cursor + i * stride, 0, v[i], 0);
      }
    }
  }

  static bool decode(BytesSpan in, typename value_of<dyn_array<T>>::type& out, Error* e=nullptr){
    if (in.size() < 32){ if(e) e->message="dyn_array: short"; return false; }

    size_t len = static_cast<size_t>(read_u256_big(in.data()).convert_to<unsigned long long>());
    out.resize(len);

    if constexpr (!elem_dyn) {
      const size_t need = 32 + 32*traits<T>::head_words*len;
      if (in.size() < need){ if(e) e->message="dyn_array: inline short"; return false; }
      size_t off = 32;
      for (size_t i = 0; i < len; ++i) {
        if (!traits<T>::decode(BytesSpan(in.data()+off, 32*traits<T>::head_words), out[i], e)) return false;
        off += 32 * traits<T>::head_words;
      }
      return true;
    } else {
      // (keep your corrected dynamic-element decode with base = 32 and bounded slices)
      const size_t head_bytes = 32 + 32*len;
      if (in.size() < head_bytes){ if(e) e->message="dyn_array: offs short"; return false; }

      const size_t base = 32;
      std::vector<size_t> offs(len + 1);
      for (size_t i=0;i<len;++i){
        size_t oi = static_cast<size_t>(read_u256_big(in.data()+32+32*i).convert_to<unsigned long long>());
        if (base + oi > in.size()){ if(e) e->message="dyn_array: bad off"; return false; }
        offs[i] = oi;
      }
      offs[len] = in.size() - base;

      for (size_t i=0;i<len;++i){
        size_t start_rel = offs[i], end_rel = offs[i+1];
        if (end_rel < start_rel){ if(e) e->message="dyn_array: off order"; return false; }
        size_t start_abs = base + start_rel, span = end_rel - start_rel;
        if (!traits<T>::decode(BytesSpan(in.data()+start_abs, span), out[i], e)) return false;
      }
      return true;
    }
  }
};

// tuple
template<class... Ts>
struct traits<tuple<Ts...>>{
  static constexpr bool is_dynamic =
      (traits<Ts>::is_dynamic || ...);

  // when the tuple itself is an element
  static constexpr size_t head_words =
      is_dynamic ? 1 : (traits<Ts>::head_words + ... + 0);

  // internal head layout size when dynamic:
  // dynamic elements contribute 1 word (offset),
  // static elements contribute their full head_words
  static constexpr size_t head_words_static =
      ((traits<Ts>::is_dynamic ? 1 : traits<Ts>::head_words) + ... + 0);

  // ----- tail_size -----
  template<class V>
  static size_t tail_size(const V& v){
    if(!is_dynamic) return 0;
    size_t bytes = 32 * head_words_static;
    tail_size_impl(v, bytes, std::make_index_sequence<sizeof...(Ts)>{});
    return bytes;
  }

  // ----- encode_head -----
  template<class V>
  static void encode_head(uint8_t* out32, size_t /*head_idx*/, const V& v, size_t base){
    if(!is_dynamic){
      size_t cur = 0;
      encode_head_static_impl(out32, cur, v, std::make_index_sequence<sizeof...(Ts)>{});
    }else{
      write_u256_imm(out32, base);
    }
  }

  // ----- encode_tail -----
  template<class V>
  static void encode_tail(uint8_t* out, size_t base, const V& v){
    if(!is_dynamic) return;
    size_t head_cursor  = 0;
    size_t running_tail = 32 * head_words_static;
    encode_tail_dyn_impl(out, base, head_cursor, running_tail, v,
                         std::make_index_sequence<sizeof...(Ts)>{});
  }

  // ----- decode -----

  static bool decode(BytesSpan in,
                     typename value_of<tuple<Ts...>>::type& out,
                     Error* e=nullptr)
  {
    // -------- static tuple: heads inline, no offsets
    if constexpr (!is_dynamic) {
      constexpr size_t total_words = (traits<Ts>::head_words + ... + 0);
      const size_t need = 32 * total_words;
      if (in.size() < need) { if (e) e->message = "tuple: head short"; return false; }

      bool ok = true;
      size_t off = 0;

      // Iterate by index so we can use the SCHEMA at each I.
      [&]<std::size_t... I>(std::index_sequence<I...>) {
        (([&]{
            using Sch = typename std::tuple_element<I, std::tuple<Ts...>>::type;
            constexpr size_t words = traits<Sch>::head_words;
            ok = ok && traits<Sch>::decode(BytesSpan(in.data() + off, 32 * words),
                                           std::get<I>(out), e);
            off += 32 * words;
          }()), ...);
      }(std::make_index_sequence<sizeof...(Ts)>{});

      return ok;
    }

    // -------- dynamic tuple: local head of size 32 * head_words_static
    const size_t need = 32 * head_words_static;
    if (in.size() < need) { if (e) e->message = "tuple(dyn): head short"; return false; }

    // Pass 1: collect offsets for dynamic elements (relative to start of this tuple slice)
    std::array<size_t, sizeof...(Ts)> dyn_off{};
    std::array<bool,   sizeof...(Ts)> dyn_is{}; dyn_is.fill(false);

    bool ok = true;
    size_t head_cursor = 0;

    [&]<std::size_t... I>(std::index_sequence<I...>) {
      (([&]{
          using Sch = typename std::tuple_element<I, std::tuple<Ts...>>::type;
          if constexpr (traits<Sch>::is_dynamic) {
            // Read local offset
            const size_t off = static_cast<size_t>(
                read_u256_big(in.data() + 32 * head_cursor).convert_to<unsigned long long>());

            // Basic sanity: tail starts at or after head; and within the buffer.
            if (off < need || off > in.size()) {
              ok = false; if (e) e->message = "tuple(dyn): bad off"; return;
            }
            dyn_off[I] = off;
            dyn_is[I]  = true;
            head_cursor += 1;                         // dynamic field = 1 word in head
          } else {
            head_cursor += traits<Sch>::head_words;   // static field = full head_words
          }
        }()), ...);
    }(std::make_index_sequence<sizeof...(Ts)>{});

    if (!ok) return false;

    // Pass 2: decode elements. Static from the head; dynamic from their offset to end.
    head_cursor = 0;

    [&]<std::size_t... I>(std::index_sequence<I...>) {
      (([&]{
          using Sch = typename std::tuple_element<I, std::tuple<Ts...>>::type;

          if constexpr (traits<Sch>::is_dynamic) {
            const size_t off = dyn_off[I];
            const size_t span = (off <= in.size()) ? (in.size() - off) : 0;
            if (span == 0) { ok = false; if (e) e->message = "tuple(dyn): tail short"; return; }
            ok = ok && traits<Sch>::decode(BytesSpan(in.data() + off, span),
                                           std::get<I>(out), e);
            head_cursor += 1;
          } else {
            constexpr size_t words = traits<Sch>::head_words;
            const size_t bytes = 32 * words;
            if (32 * head_cursor + bytes > in.size()) {
              ok = false; if (e) e->message = "tuple(dyn): static overrun"; return;
            }
            ok = ok && traits<Sch>::decode(BytesSpan(in.data() + 32 * head_cursor, bytes),
                                           std::get<I>(out), e);
            head_cursor += words;
          }
        }()), ...);
    }(std::make_index_sequence<sizeof...(Ts)>{});

    return ok;
  }

private:
  // ---- tail_size impl ----
  template<class V, size_t... I>
  static void tail_size_impl(const V& v, size_t& bytes, std::index_sequence<I...>){
    (void)std::initializer_list<int>{
      ( [&](){
          using S = typename std::tuple_element<I, std::tuple<Ts...>>::type;
          if constexpr (traits<S>::is_dynamic){
            bytes += traits<S>::tail_size(std::get<I>(v));
          }
        }(), 0 )...
    };
  }

  // ---- encode_head (!is_dynamic) ----
  template<class V, size_t... I>
  static void encode_head_static_impl(uint8_t* out32, size_t& cur, const V& v, std::index_sequence<I...>){
    (void)std::initializer_list<int>{
      ( [&](){
          using S = typename std::tuple_element<I, std::tuple<Ts...>>::type;
          traits<S>::encode_head(out32 + 32*cur, 0, std::get<I>(v), 0);
          cur += traits<S>::head_words;
        }(), 0 )...
    };
  }

  // ---- encode_tail (is_dynamic) ----
  template<class V, size_t... I>
  static void encode_tail_dyn_impl(uint8_t* out, size_t base,
                                   size_t& head_cursor, size_t& running_tail,
                                   const V& v, std::index_sequence<I...>){
    (void)std::initializer_list<int>{
      ( [&](){
          using S = typename std::tuple_element<I, std::tuple<Ts...>>::type;
          const auto& elem = std::get<I>(v);
          if constexpr (traits<S>::is_dynamic){
            write_u256_imm(out + base + 32*head_cursor, running_tail);
            traits<S>::encode_tail(out, base + running_tail, elem);
            ++head_cursor;
            running_tail += traits<S>::tail_size(elem);
          }else{
            traits<S>::encode_head(out + base + 32*head_cursor, 0, elem, 0);
            head_cursor += traits<S>::head_words;
          }
        }(), 0 )...
    };
  }

  // ---- decode (!is_dynamic) ----
  template<size_t... I>
  static void decode_static_impl(BytesSpan in,
                                 typename value_of<tuple<Ts...>>::type& out,
                                 size_t& off, bool& ok,
                                 std::index_sequence<I...>, Error* e){
    (void)std::initializer_list<int>{
      ( [&](){
          using S = typename std::tuple_element<I, std::tuple<Ts...>>::type;
          auto& elem = std::get<I>(out);
          ok = ok && traits<S>::decode(BytesSpan(in.data()+off, 32*traits<S>::head_words), elem, e);
          off += 32*traits<S>::head_words;
        }(), 0 )...
    };
  }

  // ---- decode (is_dynamic) ----
  template<size_t... I>
  static void decode_dyn_impl(BytesSpan in,
                              typename value_of<tuple<Ts...>>::type& out,
                              size_t need, size_t& head_cursor, bool& ok,
                              std::index_sequence<I...>, Error* e){
    (void)std::initializer_list<int>{
      ( [&](){
          using S = typename std::tuple_element<I, std::tuple<Ts...>>::type;
          auto& elem = std::get<I>(out);
          if constexpr (traits<S>::is_dynamic){
            size_t off = static_cast<size_t>(read_u256_big(in.data() + 32*head_cursor).convert_to<unsigned long long>());
            if(need + off > in.size()){ ok=false; return; }
            ok = ok && traits<S>::decode(BytesSpan(in.data()+need+off, in.size()-(need+off)), elem, e);
            ++head_cursor;
          }else{
            ok = ok && traits<S>::decode(BytesSpan(in.data()+32*head_cursor, 32*traits<S>::head_words), elem, e);
            head_cursor += traits<S>::head_words;
          }
        }(), 0 )...
    };
  }
};

// ----------------- arg planner & encoder -----------------
template<class... Schemas, class... Vs>
inline size_t encoded_size_args(const std::tuple<Vs...>& args){
  static_assert(sizeof...(Schemas)==sizeof...(Vs), "arity mismatch");
  constexpr size_t head_words_total =
      ((traits<Schemas>::is_dynamic ? 1 : traits<Schemas>::head_words) + ... + 0);
  size_t head_bytes = 32 * head_words_total;
  size_t tail_bytes = 0;
  std::apply([&](const auto&... vs){
    (void)std::initializer_list<int>{ ( (tail_bytes += traits<Schemas>::tail_size(vs)), 0 )... };
  }, args);
  return 4 + head_bytes + tail_bytes; // 4 for selector
}

template<class... Schemas, class... Vs>
inline bool encode_args_into(uint8_t* out, size_t out_cap,
                             const std::array<uint8_t,4>& selector,
                             const std::tuple<Vs...>& args,
                             Error* e=nullptr){
  static_assert(sizeof...(Schemas)==sizeof...(Vs), "arity mismatch");
  const size_t need = encoded_size_args<Schemas...>(args);
  if(out_cap < need){ if(e) e->message="encode: buffer too small"; return false; }

  // selector
  std::memcpy(out, selector.data(), 4);
  uint8_t* head32 = out + 4;

  // compute total head words
  constexpr size_t head_words_total =
      ((traits<Schemas>::is_dynamic ? 1 : traits<Schemas>::head_words) + ... + 0);
  const size_t base = 32 * head_words_total;

  // heads (write per-arg offset = base + running_so_far)
  size_t head_cursor = 0;
  size_t running = 0;
  std::apply([&](const auto&... vs){
    (void)std::initializer_list<int>{
      ( [&](){
          traits<Schemas>::encode_head(head32 + 32*head_cursor, 0, vs, base + running);
          head_cursor += (traits<Schemas>::is_dynamic ? 1 : traits<Schemas>::head_words);
          running += traits<Schemas>::tail_size(vs);
        }(), 0 )...
    };
  }, args);

  // tails
  running = 0;
  std::apply([&](const auto&... vs){
    (void)std::initializer_list<int>{
      ( (traits<Schemas>::encode_tail(head32, base + running, vs),
         running += traits<Schemas>::tail_size(vs)), 0 )...
    };
  }, args);
  return true;
}

// ----------------- result decoder -----------------
template<class Schema, class Out>
inline bool decode_from(BytesSpan in, Out& out, Error* e=nullptr){

  if constexpr (needs_top_ptr<Schema>::value) {
    if (in.size() < 32){ if(e) e->message = "top: short"; return false; }
    size_t off = static_cast<size_t>(read_u256_big(in.data()).convert_to<unsigned long long>());
    if (off > in.size()){ if(e) e->message = "top: bad off"; return false; }
    return traits<Schema>::decode(BytesSpan(in.data()+off, in.size()-off), out, e);
  } else {
    return traits<Schema>::decode(in, out, e);
  }
}

// ----------------- Fn wrapper (C++17 selector type) -----------------
// Provide a Selector type with: static constexpr std::array<uint8_t,4> value;
template<class Selector, class RetSchema, class... ArgSchemas>
struct Fn {
  using return_t = typename value_of<RetSchema>::type;

  template<class... Vs>
  static size_t encoded_size(const Vs&... vs){
    auto tup = std::forward_as_tuple(vs...);
    return encoded_size_args<ArgSchemas...>(tup);
  }
  template<class... Vs>
  static bool encode_into(uint8_t* out, size_t cap, const Vs&... vs, Error* e=nullptr){
    auto tup = std::forward_as_tuple(vs...);
    return encode_args_into<ArgSchemas...>(out, cap, Selector::value, tup, e);
  }
  static bool decode(BytesSpan in, return_t& out, Error* e=nullptr){
    return decode_from<RetSchema>(in, out, e);
  }
};



} // namespace abi
