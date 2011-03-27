# Simple Client / Server
## using select

### Why

Because I didn't remember how and would like to have some decent working sample code around

### How to use it

Just 'make' and you can run the server, then run the client. The server complains about 'no data' by default every 2.5 seconds, and correctly responds to incoming messages of characters up to 256 in length. You can gracefully kill the server by sending 

./client die

and you can send custom messages by sending

./client this is a message

The server and the client will both print out termination messages and lots of debug data.

### What if it breaks!

There is no warrantee or guarantee on any of this code; it's just a sample for my own use. It's got pieces derived from Beginning Linux Programming, 4th edition, by Matthew and Stone, pieces from Professor Dave Calvert at the University of Guelph, and pieces from Beej's Guide to Network Programming (specifically this page: http://beej.us/guide/bgnet/output/html/multipage/advanced.html#blocking), and my own pretty brain.

If you want to ask me a question, send it to carss.w@gmail.com
