// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: timestamp.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "timestamp.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace flib {
namespace proto {

namespace {

const ::google::protobuf::Descriptor* Timestamp_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Timestamp_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_timestamp_2eproto() {
  protobuf_AddDesc_timestamp_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "timestamp.proto");
  GOOGLE_CHECK(file != NULL);
  Timestamp_descriptor_ = file->message_type(0);
  static const int Timestamp_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Timestamp, seconds_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Timestamp, nanos_),
  };
  Timestamp_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Timestamp_descriptor_,
      Timestamp::default_instance_,
      Timestamp_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Timestamp, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Timestamp, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Timestamp));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_timestamp_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Timestamp_descriptor_, &Timestamp::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_timestamp_2eproto() {
  delete Timestamp::default_instance_;
  delete Timestamp_reflection_;
}

void protobuf_AddDesc_timestamp_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::flib::proto::protobuf_AddDesc_common_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\017timestamp.proto\022\nflib.proto\032\014common.pr"
    "oto\"+\n\tTimestamp\022\017\n\007seconds\030\001 \002(\003\022\r\n\005nan"
    "os\030\002 \002(\005", 88);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "timestamp.proto", &protobuf_RegisterTypes);
  Timestamp::default_instance_ = new Timestamp();
  Timestamp::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_timestamp_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_timestamp_2eproto {
  StaticDescriptorInitializer_timestamp_2eproto() {
    protobuf_AddDesc_timestamp_2eproto();
  }
} static_descriptor_initializer_timestamp_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int Timestamp::kSecondsFieldNumber;
const int Timestamp::kNanosFieldNumber;
#endif  // !_MSC_VER

Timestamp::Timestamp()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:flib.proto.Timestamp)
}

void Timestamp::InitAsDefaultInstance() {
}

Timestamp::Timestamp(const Timestamp& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:flib.proto.Timestamp)
}

void Timestamp::SharedCtor() {
  _cached_size_ = 0;
  seconds_ = GOOGLE_LONGLONG(0);
  nanos_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Timestamp::~Timestamp() {
  // @@protoc_insertion_point(destructor:flib.proto.Timestamp)
  SharedDtor();
}

void Timestamp::SharedDtor() {
  if (this != default_instance_) {
  }
}

void Timestamp::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Timestamp::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Timestamp_descriptor_;
}

const Timestamp& Timestamp::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_timestamp_2eproto();
  return *default_instance_;
}

Timestamp* Timestamp::default_instance_ = NULL;

Timestamp* Timestamp::New() const {
  return new Timestamp;
}

void Timestamp::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<Timestamp*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  ZR_(seconds_, nanos_);

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Timestamp::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:flib.proto.Timestamp)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int64 seconds = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &seconds_)));
          set_has_seconds();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_nanos;
        break;
      }

      // required int32 nanos = 2;
      case 2: {
        if (tag == 16) {
         parse_nanos:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &nanos_)));
          set_has_nanos();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:flib.proto.Timestamp)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:flib.proto.Timestamp)
  return false;
#undef DO_
}

void Timestamp::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:flib.proto.Timestamp)
  // required int64 seconds = 1;
  if (has_seconds()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(1, this->seconds(), output);
  }

  // required int32 nanos = 2;
  if (has_nanos()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->nanos(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:flib.proto.Timestamp)
}

::google::protobuf::uint8* Timestamp::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:flib.proto.Timestamp)
  // required int64 seconds = 1;
  if (has_seconds()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(1, this->seconds(), target);
  }

  // required int32 nanos = 2;
  if (has_nanos()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->nanos(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:flib.proto.Timestamp)
  return target;
}

int Timestamp::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int64 seconds = 1;
    if (has_seconds()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->seconds());
    }

    // required int32 nanos = 2;
    if (has_nanos()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->nanos());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Timestamp::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Timestamp* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Timestamp*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Timestamp::MergeFrom(const Timestamp& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_seconds()) {
      set_seconds(from.seconds());
    }
    if (from.has_nanos()) {
      set_nanos(from.nanos());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Timestamp::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Timestamp::CopyFrom(const Timestamp& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Timestamp::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void Timestamp::Swap(Timestamp* other) {
  if (other != this) {
    std::swap(seconds_, other->seconds_);
    std::swap(nanos_, other->nanos_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Timestamp::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Timestamp_descriptor_;
  metadata.reflection = Timestamp_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace proto
}  // namespace flib

// @@protoc_insertion_point(global_scope)
