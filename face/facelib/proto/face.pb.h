// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: face.proto

#ifndef PROTOBUF_face_2eproto__INCLUDED
#define PROTOBUF_face_2eproto__INCLUDED

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
#include "rect.pb.h"
#include "landmark.pb.h"
#include "mat.pb.h"
// @@protoc_insertion_point(includes)

namespace flib {
namespace proto {

// Internal implementation detail -- do not call these.
void FACELIBSHARED_EXPORT protobuf_AddDesc_face_2eproto();
void protobuf_AssignDesc_face_2eproto();
void protobuf_ShutdownFile_face_2eproto();

class Face;

// ===================================================================

class FACELIBSHARED_EXPORT Face : public ::google::protobuf::Message {
 public:
  Face();
  virtual ~Face();

  Face(const Face& from);

  inline Face& operator=(const Face& from) {
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
  static const Face& default_instance();

  void Swap(Face* other);

  // implements Message ----------------------------------------------

  Face* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Face& from);
  void MergeFrom(const Face& from);
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

  // optional string track_uuid = 1;
  inline bool has_track_uuid() const;
  inline void clear_track_uuid();
  static const int kTrackUuidFieldNumber = 1;
  inline const ::std::string& track_uuid() const;
  inline void set_track_uuid(const ::std::string& value);
  inline void set_track_uuid(const char* value);
  inline void set_track_uuid(const char* value, size_t size);
  inline ::std::string* mutable_track_uuid();
  inline ::std::string* release_track_uuid();
  inline void set_allocated_track_uuid(::std::string* track_uuid);

  // required .flib.proto.Rect rect = 2;
  inline bool has_rect() const;
  inline void clear_rect();
  static const int kRectFieldNumber = 2;
  inline const ::flib::proto::Rect& rect() const;
  inline ::flib::proto::Rect* mutable_rect();
  inline ::flib::proto::Rect* release_rect();
  inline void set_allocated_rect(::flib::proto::Rect* rect);

  // repeated .flib.proto.Landmark landmarks = 3;
  inline int landmarks_size() const;
  inline void clear_landmarks();
  static const int kLandmarksFieldNumber = 3;
  inline const ::flib::proto::Landmark& landmarks(int index) const;
  inline ::flib::proto::Landmark* mutable_landmarks(int index);
  inline ::flib::proto::Landmark* add_landmarks();
  inline const ::google::protobuf::RepeatedPtrField< ::flib::proto::Landmark >&
      landmarks() const;
  inline ::google::protobuf::RepeatedPtrField< ::flib::proto::Landmark >*
      mutable_landmarks();

  // optional .flib.proto.Mat mat = 4;
  inline bool has_mat() const;
  inline void clear_mat();
  static const int kMatFieldNumber = 4;
  inline const ::flib::proto::Mat& mat() const;
  inline ::flib::proto::Mat* mutable_mat();
  inline ::flib::proto::Mat* release_mat();
  inline void set_allocated_mat(::flib::proto::Mat* mat);

  // @@protoc_insertion_point(class_scope:flib.proto.Face)
 private:
  inline void set_has_track_uuid();
  inline void clear_has_track_uuid();
  inline void set_has_rect();
  inline void clear_has_rect();
  inline void set_has_mat();
  inline void clear_has_mat();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* track_uuid_;
  ::flib::proto::Rect* rect_;
  ::google::protobuf::RepeatedPtrField< ::flib::proto::Landmark > landmarks_;
  ::flib::proto::Mat* mat_;
  friend void FACELIBSHARED_EXPORT protobuf_AddDesc_face_2eproto();
  friend void protobuf_AssignDesc_face_2eproto();
  friend void protobuf_ShutdownFile_face_2eproto();

  void InitAsDefaultInstance();
  static Face* default_instance_;
};
// ===================================================================


// ===================================================================

// Face

// optional string track_uuid = 1;
inline bool Face::has_track_uuid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Face::set_has_track_uuid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Face::clear_has_track_uuid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Face::clear_track_uuid() {
  if (track_uuid_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    track_uuid_->clear();
  }
  clear_has_track_uuid();
}
inline const ::std::string& Face::track_uuid() const {
  // @@protoc_insertion_point(field_get:flib.proto.Face.track_uuid)
  return *track_uuid_;
}
inline void Face::set_track_uuid(const ::std::string& value) {
  set_has_track_uuid();
  if (track_uuid_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    track_uuid_ = new ::std::string;
  }
  track_uuid_->assign(value);
  // @@protoc_insertion_point(field_set:flib.proto.Face.track_uuid)
}
inline void Face::set_track_uuid(const char* value) {
  set_has_track_uuid();
  if (track_uuid_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    track_uuid_ = new ::std::string;
  }
  track_uuid_->assign(value);
  // @@protoc_insertion_point(field_set_char:flib.proto.Face.track_uuid)
}
inline void Face::set_track_uuid(const char* value, size_t size) {
  set_has_track_uuid();
  if (track_uuid_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    track_uuid_ = new ::std::string;
  }
  track_uuid_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:flib.proto.Face.track_uuid)
}
inline ::std::string* Face::mutable_track_uuid() {
  set_has_track_uuid();
  if (track_uuid_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    track_uuid_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:flib.proto.Face.track_uuid)
  return track_uuid_;
}
inline ::std::string* Face::release_track_uuid() {
  clear_has_track_uuid();
  if (track_uuid_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = track_uuid_;
    track_uuid_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void Face::set_allocated_track_uuid(::std::string* track_uuid) {
  if (track_uuid_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete track_uuid_;
  }
  if (track_uuid) {
    set_has_track_uuid();
    track_uuid_ = track_uuid;
  } else {
    clear_has_track_uuid();
    track_uuid_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:flib.proto.Face.track_uuid)
}

// required .flib.proto.Rect rect = 2;
inline bool Face::has_rect() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Face::set_has_rect() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Face::clear_has_rect() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Face::clear_rect() {
  if (rect_ != NULL) rect_->::flib::proto::Rect::Clear();
  clear_has_rect();
}
inline const ::flib::proto::Rect& Face::rect() const {
  // @@protoc_insertion_point(field_get:flib.proto.Face.rect)
  return rect_ != NULL ? *rect_ : *default_instance_->rect_;
}
inline ::flib::proto::Rect* Face::mutable_rect() {
  set_has_rect();
  if (rect_ == NULL) rect_ = new ::flib::proto::Rect;
  // @@protoc_insertion_point(field_mutable:flib.proto.Face.rect)
  return rect_;
}
inline ::flib::proto::Rect* Face::release_rect() {
  clear_has_rect();
  ::flib::proto::Rect* temp = rect_;
  rect_ = NULL;
  return temp;
}
inline void Face::set_allocated_rect(::flib::proto::Rect* rect) {
  delete rect_;
  rect_ = rect;
  if (rect) {
    set_has_rect();
  } else {
    clear_has_rect();
  }
  // @@protoc_insertion_point(field_set_allocated:flib.proto.Face.rect)
}

// repeated .flib.proto.Landmark landmarks = 3;
inline int Face::landmarks_size() const {
  return landmarks_.size();
}
inline void Face::clear_landmarks() {
  landmarks_.Clear();
}
inline const ::flib::proto::Landmark& Face::landmarks(int index) const {
  // @@protoc_insertion_point(field_get:flib.proto.Face.landmarks)
  return landmarks_.Get(index);
}
inline ::flib::proto::Landmark* Face::mutable_landmarks(int index) {
  // @@protoc_insertion_point(field_mutable:flib.proto.Face.landmarks)
  return landmarks_.Mutable(index);
}
inline ::flib::proto::Landmark* Face::add_landmarks() {
  // @@protoc_insertion_point(field_add:flib.proto.Face.landmarks)
  return landmarks_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::flib::proto::Landmark >&
Face::landmarks() const {
  // @@protoc_insertion_point(field_list:flib.proto.Face.landmarks)
  return landmarks_;
}
inline ::google::protobuf::RepeatedPtrField< ::flib::proto::Landmark >*
Face::mutable_landmarks() {
  // @@protoc_insertion_point(field_mutable_list:flib.proto.Face.landmarks)
  return &landmarks_;
}

// optional .flib.proto.Mat mat = 4;
inline bool Face::has_mat() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Face::set_has_mat() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Face::clear_has_mat() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Face::clear_mat() {
  if (mat_ != NULL) mat_->::flib::proto::Mat::Clear();
  clear_has_mat();
}
inline const ::flib::proto::Mat& Face::mat() const {
  // @@protoc_insertion_point(field_get:flib.proto.Face.mat)
  return mat_ != NULL ? *mat_ : *default_instance_->mat_;
}
inline ::flib::proto::Mat* Face::mutable_mat() {
  set_has_mat();
  if (mat_ == NULL) mat_ = new ::flib::proto::Mat;
  // @@protoc_insertion_point(field_mutable:flib.proto.Face.mat)
  return mat_;
}
inline ::flib::proto::Mat* Face::release_mat() {
  clear_has_mat();
  ::flib::proto::Mat* temp = mat_;
  mat_ = NULL;
  return temp;
}
inline void Face::set_allocated_mat(::flib::proto::Mat* mat) {
  delete mat_;
  mat_ = mat;
  if (mat) {
    set_has_mat();
  } else {
    clear_has_mat();
  }
  // @@protoc_insertion_point(field_set_allocated:flib.proto.Face.mat)
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

#endif  // PROTOBUF_face_2eproto__INCLUDED
