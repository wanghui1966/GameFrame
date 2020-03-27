// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: common.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_common_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_common_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3011000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3011004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_common_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_common_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[3]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_common_2eproto;
namespace PB {
class common_info;
class common_infoDefaultTypeInternal;
extern common_infoDefaultTypeInternal _common_info_default_instance_;
class dt_get_common_info_req;
class dt_get_common_info_reqDefaultTypeInternal;
extern dt_get_common_info_reqDefaultTypeInternal _dt_get_common_info_req_default_instance_;
class dt_get_common_info_rsp;
class dt_get_common_info_rspDefaultTypeInternal;
extern dt_get_common_info_rspDefaultTypeInternal _dt_get_common_info_rsp_default_instance_;
}  // namespace PB
PROTOBUF_NAMESPACE_OPEN
template<> ::PB::common_info* Arena::CreateMaybeMessage<::PB::common_info>(Arena*);
template<> ::PB::dt_get_common_info_req* Arena::CreateMaybeMessage<::PB::dt_get_common_info_req>(Arena*);
template<> ::PB::dt_get_common_info_rsp* Arena::CreateMaybeMessage<::PB::dt_get_common_info_rsp>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace PB {

enum DATA_TYPE : int {
  DT_GET_COMMON_INFO_REQ = 0,
  DT_GET_COMMON_INFO_RSP = 1
};
bool DATA_TYPE_IsValid(int value);
constexpr DATA_TYPE DATA_TYPE_MIN = DT_GET_COMMON_INFO_REQ;
constexpr DATA_TYPE DATA_TYPE_MAX = DT_GET_COMMON_INFO_RSP;
constexpr int DATA_TYPE_ARRAYSIZE = DATA_TYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* DATA_TYPE_descriptor();
template<typename T>
inline const std::string& DATA_TYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, DATA_TYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function DATA_TYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    DATA_TYPE_descriptor(), enum_t_value);
}
inline bool DATA_TYPE_Parse(
    const std::string& name, DATA_TYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<DATA_TYPE>(
    DATA_TYPE_descriptor(), name, value);
}
// ===================================================================

class dt_get_common_info_req :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:PB.dt_get_common_info_req) */ {
 public:
  dt_get_common_info_req();
  virtual ~dt_get_common_info_req();

  dt_get_common_info_req(const dt_get_common_info_req& from);
  dt_get_common_info_req(dt_get_common_info_req&& from) noexcept
    : dt_get_common_info_req() {
    *this = ::std::move(from);
  }

  inline dt_get_common_info_req& operator=(const dt_get_common_info_req& from) {
    CopyFrom(from);
    return *this;
  }
  inline dt_get_common_info_req& operator=(dt_get_common_info_req&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const dt_get_common_info_req& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const dt_get_common_info_req* internal_default_instance() {
    return reinterpret_cast<const dt_get_common_info_req*>(
               &_dt_get_common_info_req_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(dt_get_common_info_req& a, dt_get_common_info_req& b) {
    a.Swap(&b);
  }
  inline void Swap(dt_get_common_info_req* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline dt_get_common_info_req* New() const final {
    return CreateMaybeMessage<dt_get_common_info_req>(nullptr);
  }

  dt_get_common_info_req* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<dt_get_common_info_req>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const dt_get_common_info_req& from);
  void MergeFrom(const dt_get_common_info_req& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(dt_get_common_info_req* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "PB.dt_get_common_info_req";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_common_2eproto);
    return ::descriptor_table_common_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kTypeFieldNumber = 1,
    kIdFieldNumber = 2,
  };
  // optional .PB.DATA_TYPE type = 1 [default = DT_GET_COMMON_INFO_REQ];
  bool has_type() const;
  private:
  bool _internal_has_type() const;
  public:
  void clear_type();
  ::PB::DATA_TYPE type() const;
  void set_type(::PB::DATA_TYPE value);
  private:
  ::PB::DATA_TYPE _internal_type() const;
  void _internal_set_type(::PB::DATA_TYPE value);
  public:

  // optional int32 id = 2;
  bool has_id() const;
  private:
  bool _internal_has_id() const;
  public:
  void clear_id();
  ::PROTOBUF_NAMESPACE_ID::int32 id() const;
  void set_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_id() const;
  void _internal_set_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:PB.dt_get_common_info_req)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  int type_;
  ::PROTOBUF_NAMESPACE_ID::int32 id_;
  friend struct ::TableStruct_common_2eproto;
};
// -------------------------------------------------------------------

class common_info :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:PB.common_info) */ {
 public:
  common_info();
  virtual ~common_info();

  common_info(const common_info& from);
  common_info(common_info&& from) noexcept
    : common_info() {
    *this = ::std::move(from);
  }

  inline common_info& operator=(const common_info& from) {
    CopyFrom(from);
    return *this;
  }
  inline common_info& operator=(common_info&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const common_info& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const common_info* internal_default_instance() {
    return reinterpret_cast<const common_info*>(
               &_common_info_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(common_info& a, common_info& b) {
    a.Swap(&b);
  }
  inline void Swap(common_info* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline common_info* New() const final {
    return CreateMaybeMessage<common_info>(nullptr);
  }

  common_info* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<common_info>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const common_info& from);
  void MergeFrom(const common_info& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(common_info* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "PB.common_info";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_common_2eproto);
    return ::descriptor_table_common_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kRiFieldNumber = 3,
    kStrFieldNumber = 2,
    kFlagFieldNumber = 1,
  };
  // repeated int64 ri = 3;
  int ri_size() const;
  private:
  int _internal_ri_size() const;
  public:
  void clear_ri();
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_ri(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int64 >&
      _internal_ri() const;
  void _internal_add_ri(::PROTOBUF_NAMESPACE_ID::int64 value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int64 >*
      _internal_mutable_ri();
  public:
  ::PROTOBUF_NAMESPACE_ID::int64 ri(int index) const;
  void set_ri(int index, ::PROTOBUF_NAMESPACE_ID::int64 value);
  void add_ri(::PROTOBUF_NAMESPACE_ID::int64 value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int64 >&
      ri() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int64 >*
      mutable_ri();

  // optional bytes str = 2;
  bool has_str() const;
  private:
  bool _internal_has_str() const;
  public:
  void clear_str();
  const std::string& str() const;
  void set_str(const std::string& value);
  void set_str(std::string&& value);
  void set_str(const char* value);
  void set_str(const void* value, size_t size);
  std::string* mutable_str();
  std::string* release_str();
  void set_allocated_str(std::string* str);
  private:
  const std::string& _internal_str() const;
  void _internal_set_str(const std::string& value);
  std::string* _internal_mutable_str();
  public:

  // optional bool flag = 1;
  bool has_flag() const;
  private:
  bool _internal_has_flag() const;
  public:
  void clear_flag();
  bool flag() const;
  void set_flag(bool value);
  private:
  bool _internal_flag() const;
  void _internal_set_flag(bool value);
  public:

  // @@protoc_insertion_point(class_scope:PB.common_info)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int64 > ri_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr str_;
  bool flag_;
  friend struct ::TableStruct_common_2eproto;
};
// -------------------------------------------------------------------

class dt_get_common_info_rsp :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:PB.dt_get_common_info_rsp) */ {
 public:
  dt_get_common_info_rsp();
  virtual ~dt_get_common_info_rsp();

  dt_get_common_info_rsp(const dt_get_common_info_rsp& from);
  dt_get_common_info_rsp(dt_get_common_info_rsp&& from) noexcept
    : dt_get_common_info_rsp() {
    *this = ::std::move(from);
  }

  inline dt_get_common_info_rsp& operator=(const dt_get_common_info_rsp& from) {
    CopyFrom(from);
    return *this;
  }
  inline dt_get_common_info_rsp& operator=(dt_get_common_info_rsp&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const dt_get_common_info_rsp& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const dt_get_common_info_rsp* internal_default_instance() {
    return reinterpret_cast<const dt_get_common_info_rsp*>(
               &_dt_get_common_info_rsp_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(dt_get_common_info_rsp& a, dt_get_common_info_rsp& b) {
    a.Swap(&b);
  }
  inline void Swap(dt_get_common_info_rsp* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline dt_get_common_info_rsp* New() const final {
    return CreateMaybeMessage<dt_get_common_info_rsp>(nullptr);
  }

  dt_get_common_info_rsp* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<dt_get_common_info_rsp>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const dt_get_common_info_rsp& from);
  void MergeFrom(const dt_get_common_info_rsp& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(dt_get_common_info_rsp* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "PB.dt_get_common_info_rsp";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_common_2eproto);
    return ::descriptor_table_common_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kRciFieldNumber = 4,
    kOciFieldNumber = 3,
    kIdFieldNumber = 2,
    kTypeFieldNumber = 1,
  };
  // repeated .PB.common_info rci = 4;
  int rci_size() const;
  private:
  int _internal_rci_size() const;
  public:
  void clear_rci();
  ::PB::common_info* mutable_rci(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PB::common_info >*
      mutable_rci();
  private:
  const ::PB::common_info& _internal_rci(int index) const;
  ::PB::common_info* _internal_add_rci();
  public:
  const ::PB::common_info& rci(int index) const;
  ::PB::common_info* add_rci();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PB::common_info >&
      rci() const;

  // optional .PB.common_info oci = 3;
  bool has_oci() const;
  private:
  bool _internal_has_oci() const;
  public:
  void clear_oci();
  const ::PB::common_info& oci() const;
  ::PB::common_info* release_oci();
  ::PB::common_info* mutable_oci();
  void set_allocated_oci(::PB::common_info* oci);
  private:
  const ::PB::common_info& _internal_oci() const;
  ::PB::common_info* _internal_mutable_oci();
  public:

  // optional int32 id = 2;
  bool has_id() const;
  private:
  bool _internal_has_id() const;
  public:
  void clear_id();
  ::PROTOBUF_NAMESPACE_ID::int32 id() const;
  void set_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_id() const;
  void _internal_set_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // optional .PB.DATA_TYPE type = 1 [default = DT_GET_COMMON_INFO_RSP];
  bool has_type() const;
  private:
  bool _internal_has_type() const;
  public:
  void clear_type();
  ::PB::DATA_TYPE type() const;
  void set_type(::PB::DATA_TYPE value);
  private:
  ::PB::DATA_TYPE _internal_type() const;
  void _internal_set_type(::PB::DATA_TYPE value);
  public:

  // @@protoc_insertion_point(class_scope:PB.dt_get_common_info_rsp)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PB::common_info > rci_;
  ::PB::common_info* oci_;
  ::PROTOBUF_NAMESPACE_ID::int32 id_;
  int type_;
  friend struct ::TableStruct_common_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// dt_get_common_info_req

// optional .PB.DATA_TYPE type = 1 [default = DT_GET_COMMON_INFO_REQ];
inline bool dt_get_common_info_req::_internal_has_type() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool dt_get_common_info_req::has_type() const {
  return _internal_has_type();
}
inline void dt_get_common_info_req::clear_type() {
  type_ = 0;
  _has_bits_[0] &= ~0x00000001u;
}
inline ::PB::DATA_TYPE dt_get_common_info_req::_internal_type() const {
  return static_cast< ::PB::DATA_TYPE >(type_);
}
inline ::PB::DATA_TYPE dt_get_common_info_req::type() const {
  // @@protoc_insertion_point(field_get:PB.dt_get_common_info_req.type)
  return _internal_type();
}
inline void dt_get_common_info_req::_internal_set_type(::PB::DATA_TYPE value) {
  assert(::PB::DATA_TYPE_IsValid(value));
  _has_bits_[0] |= 0x00000001u;
  type_ = value;
}
inline void dt_get_common_info_req::set_type(::PB::DATA_TYPE value) {
  _internal_set_type(value);
  // @@protoc_insertion_point(field_set:PB.dt_get_common_info_req.type)
}

// optional int32 id = 2;
inline bool dt_get_common_info_req::_internal_has_id() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool dt_get_common_info_req::has_id() const {
  return _internal_has_id();
}
inline void dt_get_common_info_req::clear_id() {
  id_ = 0;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 dt_get_common_info_req::_internal_id() const {
  return id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 dt_get_common_info_req::id() const {
  // @@protoc_insertion_point(field_get:PB.dt_get_common_info_req.id)
  return _internal_id();
}
inline void dt_get_common_info_req::_internal_set_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000002u;
  id_ = value;
}
inline void dt_get_common_info_req::set_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:PB.dt_get_common_info_req.id)
}

// -------------------------------------------------------------------

// common_info

// optional bool flag = 1;
inline bool common_info::_internal_has_flag() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool common_info::has_flag() const {
  return _internal_has_flag();
}
inline void common_info::clear_flag() {
  flag_ = false;
  _has_bits_[0] &= ~0x00000002u;
}
inline bool common_info::_internal_flag() const {
  return flag_;
}
inline bool common_info::flag() const {
  // @@protoc_insertion_point(field_get:PB.common_info.flag)
  return _internal_flag();
}
inline void common_info::_internal_set_flag(bool value) {
  _has_bits_[0] |= 0x00000002u;
  flag_ = value;
}
inline void common_info::set_flag(bool value) {
  _internal_set_flag(value);
  // @@protoc_insertion_point(field_set:PB.common_info.flag)
}

// optional bytes str = 2;
inline bool common_info::_internal_has_str() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool common_info::has_str() const {
  return _internal_has_str();
}
inline void common_info::clear_str() {
  str_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  _has_bits_[0] &= ~0x00000001u;
}
inline const std::string& common_info::str() const {
  // @@protoc_insertion_point(field_get:PB.common_info.str)
  return _internal_str();
}
inline void common_info::set_str(const std::string& value) {
  _internal_set_str(value);
  // @@protoc_insertion_point(field_set:PB.common_info.str)
}
inline std::string* common_info::mutable_str() {
  // @@protoc_insertion_point(field_mutable:PB.common_info.str)
  return _internal_mutable_str();
}
inline const std::string& common_info::_internal_str() const {
  return str_.GetNoArena();
}
inline void common_info::_internal_set_str(const std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  str_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void common_info::set_str(std::string&& value) {
  _has_bits_[0] |= 0x00000001u;
  str_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:PB.common_info.str)
}
inline void common_info::set_str(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000001u;
  str_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:PB.common_info.str)
}
inline void common_info::set_str(const void* value, size_t size) {
  _has_bits_[0] |= 0x00000001u;
  str_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:PB.common_info.str)
}
inline std::string* common_info::_internal_mutable_str() {
  _has_bits_[0] |= 0x00000001u;
  return str_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* common_info::release_str() {
  // @@protoc_insertion_point(field_release:PB.common_info.str)
  if (!_internal_has_str()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  return str_.ReleaseNonDefaultNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void common_info::set_allocated_str(std::string* str) {
  if (str != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  str_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), str);
  // @@protoc_insertion_point(field_set_allocated:PB.common_info.str)
}

// repeated int64 ri = 3;
inline int common_info::_internal_ri_size() const {
  return ri_.size();
}
inline int common_info::ri_size() const {
  return _internal_ri_size();
}
inline void common_info::clear_ri() {
  ri_.Clear();
}
inline ::PROTOBUF_NAMESPACE_ID::int64 common_info::_internal_ri(int index) const {
  return ri_.Get(index);
}
inline ::PROTOBUF_NAMESPACE_ID::int64 common_info::ri(int index) const {
  // @@protoc_insertion_point(field_get:PB.common_info.ri)
  return _internal_ri(index);
}
inline void common_info::set_ri(int index, ::PROTOBUF_NAMESPACE_ID::int64 value) {
  ri_.Set(index, value);
  // @@protoc_insertion_point(field_set:PB.common_info.ri)
}
inline void common_info::_internal_add_ri(::PROTOBUF_NAMESPACE_ID::int64 value) {
  ri_.Add(value);
}
inline void common_info::add_ri(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_add_ri(value);
  // @@protoc_insertion_point(field_add:PB.common_info.ri)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int64 >&
common_info::_internal_ri() const {
  return ri_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int64 >&
common_info::ri() const {
  // @@protoc_insertion_point(field_list:PB.common_info.ri)
  return _internal_ri();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int64 >*
common_info::_internal_mutable_ri() {
  return &ri_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int64 >*
common_info::mutable_ri() {
  // @@protoc_insertion_point(field_mutable_list:PB.common_info.ri)
  return _internal_mutable_ri();
}

// -------------------------------------------------------------------

// dt_get_common_info_rsp

// optional .PB.DATA_TYPE type = 1 [default = DT_GET_COMMON_INFO_RSP];
inline bool dt_get_common_info_rsp::_internal_has_type() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool dt_get_common_info_rsp::has_type() const {
  return _internal_has_type();
}
inline void dt_get_common_info_rsp::clear_type() {
  type_ = 1;
  _has_bits_[0] &= ~0x00000004u;
}
inline ::PB::DATA_TYPE dt_get_common_info_rsp::_internal_type() const {
  return static_cast< ::PB::DATA_TYPE >(type_);
}
inline ::PB::DATA_TYPE dt_get_common_info_rsp::type() const {
  // @@protoc_insertion_point(field_get:PB.dt_get_common_info_rsp.type)
  return _internal_type();
}
inline void dt_get_common_info_rsp::_internal_set_type(::PB::DATA_TYPE value) {
  assert(::PB::DATA_TYPE_IsValid(value));
  _has_bits_[0] |= 0x00000004u;
  type_ = value;
}
inline void dt_get_common_info_rsp::set_type(::PB::DATA_TYPE value) {
  _internal_set_type(value);
  // @@protoc_insertion_point(field_set:PB.dt_get_common_info_rsp.type)
}

// optional int32 id = 2;
inline bool dt_get_common_info_rsp::_internal_has_id() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool dt_get_common_info_rsp::has_id() const {
  return _internal_has_id();
}
inline void dt_get_common_info_rsp::clear_id() {
  id_ = 0;
  _has_bits_[0] &= ~0x00000002u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 dt_get_common_info_rsp::_internal_id() const {
  return id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 dt_get_common_info_rsp::id() const {
  // @@protoc_insertion_point(field_get:PB.dt_get_common_info_rsp.id)
  return _internal_id();
}
inline void dt_get_common_info_rsp::_internal_set_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000002u;
  id_ = value;
}
inline void dt_get_common_info_rsp::set_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:PB.dt_get_common_info_rsp.id)
}

// optional .PB.common_info oci = 3;
inline bool dt_get_common_info_rsp::_internal_has_oci() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  PROTOBUF_ASSUME(!value || oci_ != nullptr);
  return value;
}
inline bool dt_get_common_info_rsp::has_oci() const {
  return _internal_has_oci();
}
inline void dt_get_common_info_rsp::clear_oci() {
  if (oci_ != nullptr) oci_->Clear();
  _has_bits_[0] &= ~0x00000001u;
}
inline const ::PB::common_info& dt_get_common_info_rsp::_internal_oci() const {
  const ::PB::common_info* p = oci_;
  return p != nullptr ? *p : *reinterpret_cast<const ::PB::common_info*>(
      &::PB::_common_info_default_instance_);
}
inline const ::PB::common_info& dt_get_common_info_rsp::oci() const {
  // @@protoc_insertion_point(field_get:PB.dt_get_common_info_rsp.oci)
  return _internal_oci();
}
inline ::PB::common_info* dt_get_common_info_rsp::release_oci() {
  // @@protoc_insertion_point(field_release:PB.dt_get_common_info_rsp.oci)
  _has_bits_[0] &= ~0x00000001u;
  ::PB::common_info* temp = oci_;
  oci_ = nullptr;
  return temp;
}
inline ::PB::common_info* dt_get_common_info_rsp::_internal_mutable_oci() {
  _has_bits_[0] |= 0x00000001u;
  if (oci_ == nullptr) {
    auto* p = CreateMaybeMessage<::PB::common_info>(GetArenaNoVirtual());
    oci_ = p;
  }
  return oci_;
}
inline ::PB::common_info* dt_get_common_info_rsp::mutable_oci() {
  // @@protoc_insertion_point(field_mutable:PB.dt_get_common_info_rsp.oci)
  return _internal_mutable_oci();
}
inline void dt_get_common_info_rsp::set_allocated_oci(::PB::common_info* oci) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete oci_;
  }
  if (oci) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena = nullptr;
    if (message_arena != submessage_arena) {
      oci = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, oci, submessage_arena);
    }
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  oci_ = oci;
  // @@protoc_insertion_point(field_set_allocated:PB.dt_get_common_info_rsp.oci)
}

// repeated .PB.common_info rci = 4;
inline int dt_get_common_info_rsp::_internal_rci_size() const {
  return rci_.size();
}
inline int dt_get_common_info_rsp::rci_size() const {
  return _internal_rci_size();
}
inline void dt_get_common_info_rsp::clear_rci() {
  rci_.Clear();
}
inline ::PB::common_info* dt_get_common_info_rsp::mutable_rci(int index) {
  // @@protoc_insertion_point(field_mutable:PB.dt_get_common_info_rsp.rci)
  return rci_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PB::common_info >*
dt_get_common_info_rsp::mutable_rci() {
  // @@protoc_insertion_point(field_mutable_list:PB.dt_get_common_info_rsp.rci)
  return &rci_;
}
inline const ::PB::common_info& dt_get_common_info_rsp::_internal_rci(int index) const {
  return rci_.Get(index);
}
inline const ::PB::common_info& dt_get_common_info_rsp::rci(int index) const {
  // @@protoc_insertion_point(field_get:PB.dt_get_common_info_rsp.rci)
  return _internal_rci(index);
}
inline ::PB::common_info* dt_get_common_info_rsp::_internal_add_rci() {
  return rci_.Add();
}
inline ::PB::common_info* dt_get_common_info_rsp::add_rci() {
  // @@protoc_insertion_point(field_add:PB.dt_get_common_info_rsp.rci)
  return _internal_add_rci();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PB::common_info >&
dt_get_common_info_rsp::rci() const {
  // @@protoc_insertion_point(field_list:PB.dt_get_common_info_rsp.rci)
  return rci_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace PB

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::PB::DATA_TYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PB::DATA_TYPE>() {
  return ::PB::DATA_TYPE_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_common_2eproto
