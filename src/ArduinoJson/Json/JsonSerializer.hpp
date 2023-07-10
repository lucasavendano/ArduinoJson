// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Json/TextFormatter.hpp>
#include <ArduinoJson/Serialization/measure.hpp>
#include <ArduinoJson/Serialization/serialize.hpp>
#include <ArduinoJson/Variant/VariantDataVisitor.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TWriter>
class JsonSerializer : public VariantDataVisitor<size_t> {
 public:
  static const bool producesText = true;

  JsonSerializer(TWriter writer) : formatter_(writer) {}

  FORCE_INLINE size_t visitArray(const ArrayData& array) {
    write('[');

    auto it = array.createIterator();

    while (!it.done()) {
      it->accept(*this);

      it.next();
      if (it.done())
        break;

      write(',');
    }

    write(']');
    return bytesWritten();
  }

  size_t visitObject(const ObjectData& object) {
    write('{');

    auto it = object.createIterator();

    while (!it.done()) {
      formatter_.writeString(it.key());
      write(':');
      it->accept(*this);

      it.next();
      if (it.done())
        break;

      write(',');
    }

    write('}');
    return bytesWritten();
  }

  size_t visitFloat(JsonFloat value) {
    formatter_.writeFloat(value);
    return bytesWritten();
  }

  size_t visitString(const char* value) {
    formatter_.writeString(value);
    return bytesWritten();
  }

  size_t visitString(JsonString value) {
    formatter_.writeString(value.c_str(), value.size());
    return bytesWritten();
  }

  size_t visitRawString(RawString value) {
    formatter_.writeRaw(value.data(), value.size());
    return bytesWritten();
  }

  size_t visitSignedInteger(JsonInteger value) {
    formatter_.writeInteger(value);
    return bytesWritten();
  }

  size_t visitUnsignedInteger(JsonUInt value) {
    formatter_.writeInteger(value);
    return bytesWritten();
  }

  size_t visitBoolean(bool value) {
    formatter_.writeBoolean(value);
    return bytesWritten();
  }

  size_t visitNull() {
    formatter_.writeRaw("null");
    return bytesWritten();
  }

 protected:
  size_t bytesWritten() const {
    return formatter_.bytesWritten();
  }

  void write(char c) {
    formatter_.writeRaw(c);
  }

  void write(const char* s) {
    formatter_.writeRaw(s);
  }

 private:
  TextFormatter<TWriter> formatter_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// Produces a minified JSON document.
// https://arduinojson.org/v6/api/json/serializejson/
template <typename TDestination>
size_t serializeJson(JsonVariantConst source, TDestination& destination) {
  using namespace detail;
  return serialize<JsonSerializer>(source, destination);
}

// Produces a minified JSON document.
// https://arduinojson.org/v6/api/json/serializejson/
inline size_t serializeJson(JsonVariantConst source, void* buffer,
                            size_t bufferSize) {
  using namespace detail;
  return serialize<JsonSerializer>(source, buffer, bufferSize);
}

// Computes the length of the document that serializeJson() produces.
// https://arduinojson.org/v6/api/json/measurejson/
inline size_t measureJson(JsonVariantConst source) {
  using namespace detail;
  return measure<JsonSerializer>(source);
}

#if ARDUINOJSON_ENABLE_STD_STREAM
template <typename T>
inline typename detail::enable_if<
    detail::is_convertible<T, JsonVariantConst>::value, std::ostream&>::type
operator<<(std::ostream& os, const T& source) {
  serializeJson(source, os);
  return os;
}
#endif

ARDUINOJSON_END_PUBLIC_NAMESPACE
