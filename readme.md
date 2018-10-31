# ownPush

[![Build Status](https://travis-ci.org/damaex/ownPush.svg?branch=master)](https://travis-ci.org/damaex/ownPush)
[![Build status](https://ci.appveyor.com/api/projects/status/j8ug21bncgq25dws/branch/master?svg=true)](https://ci.appveyor.com/project/damaex/ownpush)

## features
- [x] own [data model](data.md)
- [x] tls encryption
- [x] server
- [x] test client (c/c#)
- [ ] iOS client (obj-c)
- [ ] Android client (java)

## settings

- the settings are defined in the file `settings.ini`
- `settings.ini` has to be in the same folder as the application
- the `[ssl]` certificate is used for rest- and push-communication

### standard settings for the ini
change accordingly
```ini
[rest]
name=root
password=toor
port=7952

[ssl]
certificate=ownpush.crt
key=ownpush.key
```

## certificates

The certificates need to be placed alongside the applications or defined with relativ path in the `settings.ini`.

An example on how to create the certificates and keys can be found in the `certificates` directory.