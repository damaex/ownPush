# Data Model
## Push System
### Data Connection
- TLS Connection which must be hold
- all JSON-Data has data length as integer before hand
- All messages are build equally with type determined by the purpose.
#### Example
```json
32:{"data":"string", "purpose":int}
```
### Purpose Enumeration
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
### Login
#### Client requests login
- `data`: user identifier, unique user id
- `purpose`: Purpose::REQUEST
```json
{
	"data":"[user identifier]",
	"purpose":0
}
```
#### Server answers with challenge
- `data`: random challenge
- `purpose`: Purpose::CHALLENGE
```json
{
	"data":"[Random 64 char]",
	"purpose":1
}
```
#### Client  answers with completed challenge
- `data`: SHA2_512 of the challenge + secret
- `purpose`: Purpose::LOGIN
```json
{
	"data":"[SHA2_512(challenge + secret)]",
	"purpose":2
}
```
### Client receives Push
- `data`: the push data to handle by the client
- `purpose`: Purpose::PUSH
```json
{
	"data":"push data",
	"purpose":3
}
```
### Client receives Reset
Tells the client to reconnect
- `data`: the data will be ignored
- `purpose`: Purpose::RESET
```json
{
	"data":"",
	"purpose":4
}
```
## Rest System
- all REST commands are secured by admin login data which can be configured in the *settings.ini*
- the login data must be send as headers **name** and **password**
- all returned data is in json format
- the rest socket is secured by tls
### GET
#### /clients
- returns a list with all connected clients
#### /online
- the *"id"* must be send as inside the header
- returns if client with the specified *"id"* is online
### POST
#### /push
- the *"id"* must be send as inside the header
- the push data must be send as POST-body
- returns if push data could be sent
