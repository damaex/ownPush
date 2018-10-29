# Data Model

## Data Connection

- TLS Connection which must be hold
- all JSON-Data has data length as integer before hand
- All messages are build equally with type determined by the purpose.

### Example
```json
30:{"data":string, "purpose":int}
```

## Purpose Enumeration

```c++
enum Purpose {
    ERROR = -1,
    REQUEST = 0,
    CHALLENGE = 1,
    LOGIN = 2,
    PUSH = 3,
    RESET = 4
};
```

## Login

### Client requests login
```json
{
	"data":"004917653193752",	//user identifier
	"purpose":0					//Purpose::REQUEST
}
```
### Server answers with challenge
```json
{
	"data":"[Random 64 char]",	//random challenge
	"purpose":1					//Purpose::CHALLENGE
}
```
### Client  answers with completed challenge
```json
{
	"data":"[SHA2_512(challenge + secret)]",	//SHA2_512 of the challenge + secret
	"purpose":2									//Purpose::LOGIN
}
```

## Client receives Push
```json
{
	"data":"push data",	//the push data to handle by the client
	"purpose":3			//Purpose::PUSH
}
```

## Client receives Reset
Tells the client to reconnect
```json
{
	"data":"",	//the data will be ignored
	"purpose":4	//Purpose::RESET
}
```