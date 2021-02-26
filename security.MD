# Security overview

In this document I will review the security of this project.

## Review of security mechanisms that were employed/implemented

The following mechanisms and concepts were employed to enhance the security:
1. PKI keys, and E2E encryption
2. Safe C++ operations - Prefer static allocated buffer sizes then dynamic sizes were possible (see BytesContainer)
3. Avoiding usage of dangerous API calls (e.g. using strncpy instead of strcpy)
4. Using TCP timeout on sockets to mitigate DOS attacks
5. Limit sizes of payloads on messages


## Threat modeling 

To review the security we will review all the components of the systems, review the communication and messages, and will try to detect vulnerabilities.
We will use STRIDE to mark each vulnerability that is detected.

### Client -> Server 

#### (S)Spoofing

Can a client masquerade as another client?
Yes, the protocol does not block any user for communicate "as if" it was another user.
The attacker will not be able to read any messages (since they are encrypted), but will be able to cause those messages to be deleted by simply invoking the "get messages" request to the server.
He will also be able to send a symmetric key to other users which did not have a negotiation in place and establish a fake communication with them.
``` ISSUE_1 ```

#### (T)Tampering

Not applicable

#### (R)Repudiation

Can a user claim it did not sent a message and someone else did?
Yes, because: 1. Messages are not kept on server after those were retrieved by the clients 2. Messages are not signed

``` ISSUE_2 ```

#### (I)Information disclosure

Not applicable

#### (D)DOS

Can a client DOS the server?
Yes, since we have not limited the user actions, A user can send millions of times big files, and 
consume the entire server disk space

``` ISSUE_3 ```

#### (E)Elevation of privileges

Can a client elevate it's permission?
Since the server is not limited in it's permission, it a client manages to gain remote code execution capability on the server it will be able to use the full permissions of the server's user.

``` ISSUE_4 ```

###  Server -> Client

#### (S)Spoofing

Can a fake server masquerade as a valid server?
Yes, there is nothing that validates the server is a valid one.
An attacker can set up a fake server, and perform MitM attack or fool the users to connects to his server.
The fake server can then control the key's that are exchanged, and therefore disclose all the information passed. 

``` ISSUE_5 ```

#### (T)Tampering

Not applicable

#### (R)Repudiation

Not applicable

#### (I)Information disclosure

Not applicable

#### (D)DOS

Can a server break the client?
Yes, it can send millions of file messages causing the client machine disk space to get depleted.

``` ISSUE_6 ```

#### (E)Elevation of privileges

Can the server elevate it's permissions when communicating with the client?
Again, like on the client->server channel, if the server gains remote code execution capabilities on the client it will have the full ability as the client's running user 

``` ISSUE_7 ```

## Risk assessment 

We will not review all the issues we detected, and we will assign probability and severity, and possible mitigation steps for each one

Item | Desc | Probability | Severity | Possible mitigation
---|---|---|---|---
ISSUE_1|Client masquerade as another user|High|High|Authenticate all client requests, for example using signature on each request
ISSUE_2|User claims he did not send a message|Medium|Low|Sender should sign all messages. All messages should be kept on the server for a period of time
ISSUE_3|Client DOS of server|Low|High|Limit the users number of operations per minute
ISSUE_4|Client RCE on server|Very low|Critical|Run the server on limited user. Sandbox the server (e.g. run inside container)
ISSUE_5|Rouge/Fake server|High|High|Use server certificates and validate it's identity (i.e. use TLS with CA certificates)
ISSUE_6|Server DOS a client|Low|Low|Limit the number of messages a user can receive
ISSUE_7|Server RCE on server|Low|High|Run the client on limited user. Sandbox the client