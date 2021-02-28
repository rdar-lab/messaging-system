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
For this section we will assume that clients A and B are trying to communicate securely between them, and client C is an attacker.

### Client -> Server 

#### (S)Spoofing

Can a client masquerade as another client?
Yes, the protocol does not block any user for communicate "as if" it was another user.
An attacker will not be able to read any messages (since they are encrypted), but will be able to cause those messages to be deleted by simply invoking the "get messages" request to the server.
He will also be able to send a symmetric key to other users which did not have a negotiation in place and establish a fake communication with them.

Scenario:
Client A wants to send a covert message to client B (which client C does not want B to know). After client A sends the message, C will connect to the server but will impersonate to B (sending B's Client ID on the requests), it will issue the get messages to clear the messages queue. After that even if the real client B connects it will not get the message 

``` ISSUE_1 ```

Another issue is that if a user records a message, it can replay the same message.

Scenario:
Lets assume A and B are in war with C. And let's assume that A sent a message to B during one of the battles to retreat. C records that message. On later stages on another battle, if C sees it is about to loss it can resend that message and B will think that A sent it and retreat

``` ISSUE_2 ```

Another issue is with the fact that the IV is static and is set to 0. This means that over time, an attacker listening to the communication, can start detecting common patterns suggesting common messages sent between the parties

``` ISSUE_3 ```

#### (T)Tampering

Not applicable

#### (R)Repudiation

Can a user claim it did not sent a message and someone else did?
Yes, because: 1. Messages are not kept on server after those were retrieved by the clients 2. Messages are not signed

Scenario:
Let's assume A sends a message to B to attack C, and later on claims he did not send that message. Even if B shows him that message, A can claim that B created the message by himself, since the symmetric encryption key is known to both parties.

``` ISSUE_4 ```

#### (I)Information disclosure

Not applicable

#### (D)DOS

Can a client DOS the server?
Yes, since we have not limited the user actions, A user can send millions of times big files, and 
consume the entire server disk space

``` ISSUE_5 ```

#### (E)Elevation of privileges

Can a client elevate it's permission?
Yes, since the server is not limited in it's permission, if a client manages to gain remote code execution capability on the server it will be able to use the full permissions of the server's user.

``` ISSUE_6 ```

###  Server -> Client

#### (S)Spoofing

Can a fake server masquerade as a valid server?
Yes, there is nothing that validates the server is a valid one.
An attacker can set up a fake server, and perform MitM attack or fool the users to connects to his server.
The fake server can then control the key's that are exchanged, and therefore disclose all the information passed. 

Scenario:
Let's assume C obtains a MitM (men in the middle) capabilities on one of the parties (A or B).
C can establish a fake server, that will mimic the real server, but when asked for public keys, returns it's own key.
C will now be able to encrypt the messages send between A and B and specifically the symmetric key sent between the parties.
Once it has that he can send fake messages to any of the parties or find out the content of any messages flowing between them.

``` ISSUE_7 ```

#### (T)Tampering

Not applicable

#### (R)Repudiation

Not applicable

#### (I)Information disclosure

Not applicable

#### (D)DOS

Can a server break the client?
Yes, it can send millions of file messages causing the client machine disk space to get depleted.

``` ISSUE_8 ```

#### (E)Elevation of privileges

Can the server elevate it's permissions when communicating with the client?
Again, like on the client->server channel, if the server gains remote code execution capabilities on the client it will have the full ability as the client's running user 

``` ISSUE_9 ```

## Risk assessment 

We will now review all the issues we detected, and we will assign probability and severity, and possible mitigation steps for each one

Item | Desc | STRIDE|Probability | Severity | Possible mitigation
---|---|---|---|---|---
ISSUE_1|Client masquerade as another user|S|High|High|Authenticate all client requests, for example using signature on each request
ISSUE_2|Replay attack|S|High|High|Establish a validity token that will render replay calls invalid (for example using a signed system time token on each request)
ISSUE_3|Week encryption IV|S|Medium|Medium|Use a secure random to generate a fresh IV for each message
ISSUE_4|User claims he did not send a message|R|Medium|Low|Sender should sign all messages. All messages should be kept on the server for a period of time
ISSUE_5|Client DOS of server|D|Low|High|Limit the users number of operations per minute
ISSUE_6|Client RCE on server|E|Very low|Critical|Run the server on limited user. Sandbox the server (e.g. run inside container)
ISSUE_7|Rouge/Fake server|S|High|High|Use server certificates and validate it's identity (i.e. use TLS with CA certificates)
ISSUE_8|Server DOS a client|D|Low|Low|Limit the number of messages a user can receive
ISSUE_9|Server RCE on server|E|Low|High|Run the client on limited user. Sandbox the client
