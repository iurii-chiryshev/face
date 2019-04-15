// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: landmark.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "landmark.pb.h"

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

const ::google::protobuf::Descriptor* Landmark_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Landmark_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_landmark_2eproto() {
  protobuf_AddDesc_landmark_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "landmark.proto");
  GOOGLE_CHECK(file != NULL);
  Landmark_descriptor_ = file->message_type(0);
  static const int Landmark_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Landmark, idx_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Landmark, point_),
  };
  Landmark_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Landmark_descriptor_,
      Landmark::default_instance_,
      Landmark_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Landmark, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Landmark, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Landmark));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_landmark_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Landmark_descriptor_, &Landmark::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_landmark_2eproto() {
  delete Landmark::default_instance_;
  delete Landmark_reflection_;
}

void protobuf_AddDesc_landmark_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::flib::proto::protobuf_AddDesc_common_2eproto();
  ::flib::proto::protobuf_AddDesc_point2i_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\016landmark.proto\022\nflib.proto\032\014common.pro"
    "to\032\rpoint2i.proto\";\n\010Landmark\022\013\n\003idx\030\001 \002"
    "(\005\022\"\n\005point\030\002 \002(\0132\023.flib.proto.Point2i", 118);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "landmark.proto", &protobuf_RegisterTypes);
  Landmark::default_instance_ = new Landmark();
  Landmark::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_landmark_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_landmark_2eproto {
  StaticDescriptorInitializer_landmark_2eproto() {
    protobuf_AddDesc_landmark_2eproto();
  }
} static_descriptor_initializer_landmark_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int Landmark::kIdxFieldNumber;
const int Landmark::kPointFieldNumber;
#endif  // !_MSC_VER

Landmark::Landmark()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:flib.proto.Landmark)
}

void Landmark::InitAsDefaultInstance() {
  point_ = const_cast< ::flib::proto::Point2i*>(&::flib::proto::Point2i::default_instance());
}

Landmark::Landmark(const Landmark& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:flib.proto.Landmark)
}

void Landmark::SharedCtor() {
  _cached_size_ = 0;
  idx_ = 0;
  point_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Landmark::~Landmark() {
  // @@protoc_insertion_point(destructor:flib.proto.Landmark)
  SharedDtor();
}

void Landmark::SharedDtor() {
  if (this != default_instance_) {
    delete point_;
  }
}

void Landmark::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Landmark::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Landmark_descriptor_;
}

const Landmark& Landmark::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_landmark_2eproto();
  return *default_instance_;
}

Landmark* Landmark::default_instance_ = NULL;

Landmark* Landmark::New() const {
  return new Landmark;
}

void Landmark::Clear() {
  if (_has_bits_[0 / 32] & 3) {
    idx_ = 0;
    if (has_point()) {
      if (point_ != NULL) point_->::flib::proto::Point2i::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Landmark::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:flib.proto.Landmark)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 idx = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &idx_)));
          set_has_idx();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_point;
        break;
      }

      // required .flib.proto.Point2i point = 2;
      case 2: {
        if (tag == 18) {
         parse_point:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_point()));
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
  // @@protoc_insertion_point(parse_success:flib.proto.Landmark)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:flib.proto.Landmark)
  return false;
#undef DO_
}

void Landmark::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:flib.proto.Landmark)
  // required int32 idx = 1;
  if (has_idx()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->idx(), output);
  }

  // required .flib.proto.Point2i point = 2;
  if (has_point()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->point(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:flib.proto.Landmark)
}

::google::protobuf::uint8* Landmark::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:flib.proto.Landmark)
  // required int32 idx = 1;
  if (has_idx()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->idx(), target);
  }

  // required .flib.proto.Point2i point = 2;
  if (has_point()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->point(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:flib.proto.Landmark)
  return target;
}

int Landmark::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 idx = 1;
    if (has_idx()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->idx());
    }

    // required .flib.proto.Point2i point = 2;
    if (has_point()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->point());
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

void Landmark::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Landmark* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Landmark*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Landmark::MergeFrom(const Landmark& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_idx()) {
      set_idx(from.idx());
    }
    if (from.has_point()) {
      mutable_point()->::flib::proto::Point2i::MergeFrom(from.point());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Landmark::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Landmark::CopyFrom(const Landmark& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Landmark::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  if (has_point()) {
    if (!this->point().IsInitialized()) return false;
  }
  return true;
}

void Landmark::Swap(Landmark* other) {
  if (other != this) {
    std::swap(idx_, other->idx_);
    std::swap(point_, other->point_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Landmark::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Landmark_descriptor_;
  metadata.reflection = Landmark_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace proto
}  // namespace flib

// @@protoc_insertion_point(global_scope)