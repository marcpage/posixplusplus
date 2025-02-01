#pragma once

#include "PsxGetAddressInfoException.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

namespace psx {

class Address {
public:
    typedef std::string String;
    enum Qualified { FullyQualified, UnqualifiedLocal };
    enum Format { Numeric, Name };
    enum Required { NameRequired, NameIfAvailable };

    Address();
    virtual ~Address();

    operator struct sockaddr *();
    const struct sockaddr* get() const;
    String name(Qualified qualified=FullyQualified, Format format=Name, Required required=NameIfAvailable) const;
    virtual struct sockaddr* get()=0;
    virtual socklen_t size() const=0;
    virtual sa_family_t family() const=0;

    Address(const Address& temp_obj) = delete;
    Address& operator=(const Address& temp_obj) = delete;
protected:
    template<typename S>
    static void _memclear(S &s);
};

class IPv4 : public Address {
public:
    static const size_t Size = sizeof(sockaddr_in);
    static const sa_family_t Family = AF_INET;

    IPv4(const struct sockaddr *address, socklen_t size);
    IPv4(in_port_t port=0, u_int32_t address=INADDR_ANY);
    IPv4(const std::string &address, in_port_t port);
    IPv4(IPv4 &&other);
    virtual ~IPv4();

    struct sockaddr *get() override;
    socklen_t size() const override;
    sa_family_t family() const override;

private:
    struct sockaddr_in _address;
};

class IPv6 : public Address {
public:
    static const size_t Size = sizeof(sockaddr_in6);
    static const sa_family_t Family = AF_INET6;

    IPv6(const struct sockaddr *address, socklen_t size);
    IPv6(in_port_t port=0, in6_addr address=in6addr_any);
    IPv6(const std::string &address, in_port_t port);
    virtual ~IPv6();

    struct sockaddr *get() override;
    socklen_t size() const override;
    sa_family_t family() const override;

private:
    struct sockaddr_in6 _address;
};

inline Address::Address() {}

inline Address::~Address() {}

inline Address::operator struct sockaddr *() {
    return get();
}

inline const struct sockaddr* Address::get() const {
    return const_cast<Address *>(this)->get();
}

inline Address::String Address::name(Qualified qualified, Format format, Required required) const {
    std::string dnsName(NI_MAXHOST, '\0');
    const int flags = (UnqualifiedLocal == qualified ? NI_NOFQDN : 0) 
                    | (Numeric == format ? NI_NUMERICHOST : 0)
                    | (NameRequired == required ? NI_NAMEREQD : 0);

    GAIMessageThrow(::getnameinfo(get(), size(), const_cast<char *>(dnsName.data()), dnsName.size(), nullptr, 0, flags));
    dnsName.erase(::strlen(dnsName.data()));
    return dnsName;
}

template<typename S>
inline void Address::_memclear(S &s) {
    ::memset(&s, 0, sizeof(s));
}

inline IPv4::IPv4(const struct sockaddr *address, socklen_t size)
    :Address(), _address() {
    PsxAssert(nullptr != address);
    PsxAssert(Size == size);
    PsxAssert(Family == reinterpret_cast<const struct sockaddr *>(address)->sa_family);
    ::memcpy(&_address, address, Size);
}

inline IPv4::IPv4(in_port_t port, u_int32_t address) {
    _memclear(_address);
#if defined(__APPLE__)
  _address.sin_len = Size;
#endif
  _address.sin_family = family();
  _address.sin_addr.s_addr = address;
  _address.sin_port = htons(port);
}

inline IPv4::IPv4(const std::string &address, in_port_t port) {
    struct hostent *hostaddress;

    _memclear(_address);
    hostaddress = ::gethostbyname2(address.c_str(), family());

    if (nullptr == hostaddress) {
        PsxThrow(std::string("gethostbyname2 failed: ").append(::hstrerror(h_errno)));
    }
    PsxAssert(hostaddress->h_addrtype == family());

#if defined(__APPLE__)
    _address.sin_len = Size;
#endif
    _address.sin_family = hostaddress->h_addrtype;
    _address.sin_port = htons(port);
    ::memcpy(&_address.sin_addr, hostaddress->h_addr_list[0], static_cast<size_t>(hostaddress->h_length));
    //::memcpy(&_address.sin_addr.s_addr, hostaddress->h_addr, static_cast<size_t>(hostaddress->h_length));
    //::bcopy(reinterpret_cast<char *>(hostaddress->h_addr), reinterpret_cast<char *>(&_address.sin_addr.s_addr), static_cast<size_t>(hostaddress->h_length));
}

IPv4::IPv4(IPv4 &&other)
    :_address(other._address) {
    _memclear(other._address);
}

inline IPv4::~IPv4() {}

inline struct sockaddr *IPv4::get() {
    return reinterpret_cast<struct sockaddr *>(&_address);
}

inline socklen_t IPv4::size() const {
    return Size;
}

inline sa_family_t IPv4::family() const {
    return Family;
}

inline IPv6::IPv6(const struct sockaddr *address, socklen_t size) {
  PsxAssert(NULL != address);
  PsxAssert(Size == size);
  PsxAssert(Family == reinterpret_cast<const struct sockaddr *>(address)->sa_family);
  ::memcpy(&_address, address, Size);
}

inline IPv6::IPv6(in_port_t port, in6_addr address) {
    _memclear(_address);
#if defined(__APPLE__)
  _address.sin6_len = Size;
#endif
  _address.sin6_family = family();
  _address.sin6_addr = address;
  _address.sin6_port = htons(port);
}

inline IPv6::IPv6(const std::string &address, in_port_t port) {
    struct hostent *hostaddress;

    _memclear(_address);
    hostaddress = ::gethostbyname2(address.c_str(), family());

    if (nullptr == hostaddress) {
        PsxThrow(std::string("gethostbyname2 failed: ").append(::hstrerror(h_errno)));
    }
    PsxAssert(hostaddress->h_addrtype == family());
    
#if defined(__APPLE__)
    _address.sin6_len = Size;
#endif
    _address.sin6_family = hostaddress->h_addrtype;
    _address.sin6_port = htons(port);
    ::memcpy(&_address.sin6_addr, hostaddress->h_addr, static_cast<size_t>(hostaddress->h_length));
}

inline IPv6::~IPv6() {}

inline struct sockaddr *IPv6::get() {
    return reinterpret_cast<struct sockaddr *>(&_address);
}

inline socklen_t IPv6::size() const {
    return Size;
}

inline sa_family_t IPv6::family() const {
    return Family;
}

} // namespace psx
