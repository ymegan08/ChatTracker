# ChatTracker

Chat tracker that keeps track of the number of contributions users make to the chats.

# Functions

  // A user joins a chat
**void join(string user, string chat);**
The user joins a new or existing chat. Whether or not the user was associated with that chat before, that chat is now the user's current chat. If the user had previously joined that chat and not left it since then, the user's count of contributions to that chat remains the same; otherwise, it becomes 0. If the chat had previously been joined by this or any other user and has not been terminated since then, the chat's count of contributions is unchanged; otherwise, it becomes 0.
  // A chat is terminated
**int terminate(string chat);**
If the chat does not exist, this function returns 0; otherwise, all users associated with the chat are no longer associated with it (as if they left the chat), and the function returns the chat's count of contributions. If that chat was any user's current chat, then the existing chat the user most recently joined and hasn't left since most recenty joining becomes the user's current chat; if there is no such chat, then that user has no current chat.
  // A user contributes to that user's current chat
**int contribute(string user);**
If the user has no current chat, return 0. Otherwise, both the user's count of contributions to that user's current chat and that current chat's count of contributions are increased by one. Return the resulting user's count of contributions to that current chat.
  // A user leaves a chat
**int leave(string user, string chat);**
If the user is not associated with the indicated chat, return -1. Otherwise, the user is no longer associated with that chat, and the function returns the user's count of contributions to that chat. If that chat was the user's current chat, then the existing chat the user most recently joined and hasn't left since most recenty joining becomes the user's current chat; if there is no such chat, then that user has no current chat. This function leaves the chat's count of contributions unchanged.
  // A user leaves that user's current chat
**int leave(string user);**
If the user has no current chat, return -1. Otherwise, the user is no longer associated with that user's current chat, and the function returns the user's count of contributions made to that chat. The existing chat the user most recently joined and hasn't left since most recenty joining bcomes the user's current chat; if there is no such chat, then that user has no current chat. This function leaves the chat's count of contributions unchanged.

# Commands

j user chat, which requests a call to join(user, chat)
t chat, which requests a call to terminate(chat)
c user, which requests a call to contribute(user)
l user chat, which requests a call to leave(user, chat)
l user, which requests a call to leave(user)
