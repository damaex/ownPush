# ownPush

[![Build Status](https://travis-ci.org/damaex/ownPush.svg?branch=master)](https://travis-ci.org/damaex/ownPush)
[![Build status](https://ci.appveyor.com/api/projects/status/j8ug21bncgq25dws/branch/master?svg=true)](https://ci.appveyor.com/project/damaex/ownpush)

## features
- [x] own [data model](data.md)
- [x] tls encryption
- [x] [server](server/readme.md)
- [x] [test client](clients/testClient/readme.md)
- [ ] [iOS client](clients/ios/readme.md)
- [ ] [Android client](clients/android/readme.md)

## settings

the settings are defined in the file `settings.ini`

standard settings for the ini, change accordingly

```ini
[rest]
name=root
password=toor
port=7952

[ss]
certificate=ownpush.crt
key=ownpush.key
```

## certificates

the certificates need to be placed alongside the applications or defined with relativ path in the `settings.ini`

### create root ca
```bash
# generate the private ca key
openssl genrsa -out ca-key.pem 4096

# generate the root ca (10 years)
openssl req -x509 -new -nodes -extensions v3_ca -key ca-key.pem -days 3650 -out ca-root.pem -sha512
```

### create certificate
```bash
# generate the private certificate key
openssl genrsa -out cert-key.pem 4096

# generate certificate request
openssl req -new -key cert-key.pem -out cert.csr -sha512

# generate the certificate from the request (1 year)
openssl x509 -req -in cert.csr -CA ca-root.pem -CAkey ca-key.pem -CAcreateserial -out cert-pub.pem -days 365 -sha512
```

### create combined files
```bash
# ca.pem for use with the clients
cat ca-root.pem ca-key.pem >> ca.pem

# ownpush certificate chain for use with the server
cat ca-root.pem cert-pub.pem >> ownpush.crt

# ownpush key for use with the server
cat ca-key.pem >> ownpush.key
```