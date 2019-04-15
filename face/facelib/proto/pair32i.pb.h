// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: pair32i.proto

#ifndef PROTOBUF_pair32i_2eproto__INCLUDED
#define PROTOBUF_pair32i_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
#include "common.pb.h"
// @@protoc_insertion_point(includes)

namespace flib {
namespace proto {

// Internal implementation detail -- do not call these.
void FACELIBSHARED_EXPORT protobuf_AddDesc_pair32i_2eproto();
void protobuf_AssignDesc_pair32i_2eproto();
void protobuf_ShutdownFile_pair32i_2eproto();

class Pair32i;

// ===================================================================

class FACELIBSHARED_EXPORT Pair32i : public ::google::protobuf::Message {
 public:
  Pair32i();
  virtual ~Pair32i();

  Pair32i(const Pair32i& from);

  inline Pair32i& operator=(const Pair32i& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Pair32i& default_instance();

  void Swap(Pair32i* other);

  // implements Message ----------------------------------------------

  Pair32i* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Pair32i& from);
  void MergeFrom(const Pair32i& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 key = 1;
  inline bool has_key() const;
  inline void clear_key();
  static const int kKeyFieldNumber = 1;
  inline ::google::protobuf::int32 key() const;
  inline void set_key(::google::protobuf::int32 value);

  // required int32 value = 2;
  inline bool has_value() const;
  inline void clear_value();
  static const int kValueFieldNumber = 2;
  inline ::google::protobuf::int32 value() const;
  inline void set_value(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:flib.proto.Pair32i)
 private:
  inline void set_has_key();
  inline void clear_has_key();
  inline void set_has_value();
  inline void clear_has_value();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 key_;
  ::google::protobuf::int32 value_;
  friend void FACELIBSHARED_EXPORT protobuf_AddDesc_pair32i_2eproto();
  friend void protobuf_AssignDesc_pair32i_2eproto();
  friend void protobuf_ShutdownFile_pair32i_2eproto();

  void InitAsDefaultInstance();
  static Pair32i* default_instance_;
};
// ===================================================================


// ===================================================================

// Pair32i

// required int32 key = 1;
inline bool Pair32i::has_key() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Pair32i::set_has_key() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Pair32i::clear_has_key() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Pair32i::clear_key() {
  key_ = 0;
  clear_has_key();
}
inline ::google::protobuf::int32 Pair32i::key() const {
  // @@protoc_insertion_point(field_get:flib.proto.Pair32i.key)
  return key_;
}
inline void Pair32i::set_key(::google::protobuf::int32 value) {
  set_has_key();
  key_ = value;
  // @@protoc_insertion_point(field_set:flib.proto.Pair32i.key)
}

// required int32 value = 2;
inline bool Pair32i::has_value() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Pair32i::set_has_value() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Pair32i::clear_has_value() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Pair32i::clear_value() {
  value_ = 0;
  clear_has_value();
}
inline ::google::protobuf::int32 Pair32i::value() const {
  // @@protoc_insertion_point(field_get:flib.proto.Pair32i.value)
  return value_;
}
inline void Pair32i::set_value(::google::protobuf::int32 value) {
  set_has_value();
  value_ = value;
  // @@protoc_insertion_point(field_set:flib.proto.Pair32i.value)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace proto
}  // namespace flib

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_pair32i_2eproto__INCLUDED
