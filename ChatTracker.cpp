//
//  ChatTracker.cpp
//  PROJ4
//
//  Created by Megan Cho on 5/27/20.
//  Copyright © 2020 Yuna Cho. All rights reserved.
//

#include "ChatTracker.h"
#include <string>
#include <list>
#include <vector>
#include <functional>
using namespace std;

  // This class does the real work of the implementation.

class ChatTrackerImpl
{
  public:
    ChatTrackerImpl(int maxBuckets);
    void join(string user, string chat);    // USER JOINS CHAT
    int terminate(string chat);     // CHAT IS TERMINATED
    int contribute(string user);    // USER CONTRIBUTES TO THAT USER'S CURRENT CHAT
    int leave(string user, string chat);    // USER LEAVES A CHAT
    int leave(string user); // USER LEAVES THAT USER'S CURRENT CHAT
    // Set the maximum bucket amount for hash tables
    void setMaxBuckets(int num){
        m_maxBuckets = num;
    }
    
  private:
    int m_maxBuckets;
    struct Info // Record of data
    {
        Info(string u, string c) : user(u), chat(c), count(0) {}
        string user;
        string chat;
        int count;
    };

    vector<list <Info> > m_info;    // Use this as hash table, hash by username as key value
    vector<list <Info> > m_usersWhoLeft; // Hash table to keep track of users who left chat
    
    // Mapping function to convert to bucket key
    unsigned int mapFunc(const string& userName){
        hash<string> str_hash;  // Create string hasher
        size_t hashValue = str_hash(userName);
        unsigned int bucketNum = hashValue % m_maxBuckets;
        return bucketNum;
    }
};

ChatTrackerImpl::ChatTrackerImpl(int maxBuckets)
{
    m_info.resize(maxBuckets);  // Set bucket size = vector size
    m_usersWhoLeft.resize(maxBuckets);  // Initialize bucket size for users who leave chats
    setMaxBuckets(maxBuckets);  // Updates max bucket variable
}

void ChatTrackerImpl::join(string user, string chat)
{
    int bucket = mapFunc(user); // Compute starting bucket number
    
    // Contributions remain same if user was already in chat
    for (list<Info>::iterator li = m_info[bucket].begin(); li != m_info[bucket].end(); li++){
        // Check if user with same chat exists
        if(li->user == user && li->chat == chat){
            Info info = *li;
            m_info[bucket].erase(li);
            m_info[bucket].push_front(info);
            return;
        }
    }
    // User was not already in chat
    m_info[bucket].push_front(Info(user, chat));    // Push new chat info to user's bucket slot list
    
}

int ChatTrackerImpl::terminate(string chat) // Iterate through vector and terminate nonempty ones
{
    int totalChats = 0;
    // Iterate existing user information
    for(vector<list <Info> >::iterator i = m_info.begin(); i != m_info.end(); i++){
        list<Info>::iterator j = i->begin();
        while(j != i->end()){
            // Check if user's chat is the chat to be terminated, account for possible collision
            if(j->chat == chat){
                totalChats += j->count;
                j = i->erase(j);
            }
            else{
                j++;
            }
        }
    }
    // Iterate information of users who left
    int leftUsers = mapFunc(chat);
    list<Info>::iterator li = m_usersWhoLeft[leftUsers].begin();
    while(li != m_usersWhoLeft[leftUsers].end()){
        // Account for possible collision
        if(li->chat == chat){
            totalChats += li->count;
            li = m_usersWhoLeft[leftUsers].erase(li);
        }
        else{
            li++;
        }
    }
    return totalChats;
}

int ChatTrackerImpl::contribute(string user)
{
    int bucket = mapFunc(user); // Get user's bucket slot
    // Check if bucket has anything
    if(!m_info[bucket].empty()){
        // Iterate through bucket slot starting from most recent list elements
        for (list<Info>::iterator li = m_info[bucket].begin(); li != m_info[bucket].end(); li++){
            // Match user's most recent chat
            if(li->user == user){
                // Increment that user's contribution
                li->count++;
                // Return contribution amount
                return li->count;
            }
        }
    }
    return 0;   // Return 0 otherwise
}

int ChatTrackerImpl::leave(string user, string chat)
{
    int bucket = mapFunc(user); // Get user's bucket slot
    int leftUser = mapFunc(chat);
    // Iterate through user's chat list
    for (list<Info>::iterator li = m_info[bucket].begin(); li != m_info[bucket].end(); li++){
        // If the user is associated with the chat
        if(li->user == user && li->chat == chat){
            // Variable to store user's contribution to chat
            int userContribution = li->count;
            //string userChat = li->chat;
            // Add this user's information to corresponding bucket of users who left (hash table)
            m_usersWhoLeft[leftUser].push_front(*li);
            // Remove chat from user's list
            m_info[bucket].erase(li);
            // Return user's contribution to chat
            return userContribution;
        }
    }
    return -1;  // User not associated with chat
}

int ChatTrackerImpl::leave(string user)
{
    int bucket = mapFunc(user); // Get user's bucket slot
    // Iterate through bucket slot starting from most recent list elements
    if(!m_info[bucket].empty()){
        for (list<Info>::iterator li = m_info[bucket].begin(); li != m_info[bucket].end(); li++){
            // Match user's most recent chat
            if(li->user == user){
                // Variable to store user's contribution to chat
                int userContribution = li->count;
                string userChat = li->chat;
                int leftUser = mapFunc(userChat);
                // Add this user's information to corresponding bucket of users who left (hash table)
                m_usersWhoLeft[leftUser].push_front(*li);
                // Remove chat from user's list
                m_info[bucket].erase(li);
                // Return user's contribution to chat
                return userContribution;
            }
        }
    }
    return -1;  // User doesn't have a current chat; AKA user doesn't exist
}

//*********** ChatTracker functions **************

// These functions simply delegate to ChatTrackerImpl's functions.
// You probably don't want to change any of this code.

ChatTracker::ChatTracker(int maxBuckets)
{
    m_impl = new ChatTrackerImpl(maxBuckets);
}

ChatTracker::~ChatTracker()
{
    delete m_impl;
}

void ChatTracker::join(string user, string chat)
{
    m_impl->join(user, chat);
}

int ChatTracker::terminate(string chat)
{
    return m_impl->terminate(chat);
}

int ChatTracker::contribute(string user)
{
    return m_impl->contribute(user);
}

int ChatTracker::leave(string user, string chat)
{
    return m_impl->leave(user, chat);
}

int ChatTracker::leave(string user)
{
    return m_impl->leave(user);
}
