# Server

## REST commands

All REST commands are secured by admin login data which can be configured in the *settings.ini*.

The login data must be send as headers **name** and **password**.

All returned data is in json format.

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