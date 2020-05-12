//
// Created by danaa on 5/11/2020.
//
#include <stdlib.h>
#include <stdbool.h>
#include "votes.h"
#include "mtm_map/map.h"

Votes votesCreate () {
    Votes votes=malloc(sizeof(*votes));
    if (!votes)
    {
        return NULL;
    }
    votes->votes_num=0;
    votes->area_id=0;
    votes->tribe_id=0;
    votes->next=NULL;
    return votes;
}


Votes votePut( Votes votes, int area_id , int tribe_id , int votes_num)
{
    Votes temp=votesCreate();
    if (temp==NULL) {
        return NULL;
    }
    temp->votes_num=votes_num;
    temp->area_id=area_id;
    temp->tribe_id=tribe_id;
    temp->next=votes;
    return temp;
}

void votesDestroy (Votes votes)
{
    if (!votes){
        return;
    }
    while (votes)
    {
        Votes toDelete = votes;
        votes = votes->next;
        free(toDelete);

    }
    return;
}


Votes removeVotesByArea (Votes votes, int area_id)
{
    if (votes==NULL) {
        return NULL;
    }
    Votes temp=votes;
    while ((votes->next)!=NULL) {
      if (votes->next->area_id ==area_id)
      {
          Votes toDelete=votes->next;
          votes->next=votes->next->next;
          free(toDelete);
      }
      else {
          votes= votes->next;
      }
    }
    if (temp->area_id == area_id)
    {
        Votes toDelete= temp;
        temp=temp->next;
        free(toDelete);
    }
    return temp;
}


Votes removeVotesByTribe (Votes votes, int tribe_id)
{
    Votes temp=votes;
    while (( votes->next)!=NULL) {
        if (votes->next->tribe_id ==tribe_id)
        {
            Votes toDelete=votes->next;
            votes->next=votes->next->next;
            free(toDelete);
        }
        else {
            votes= votes->next;
        }

    }
    if (temp->tribe_id == tribe_id)
    {
        Votes toDelete= temp;
        temp=temp->next;
        free(toDelete);
    }
    return temp;
}

int minTribe ( int a , int b)
{
    if (a<b)
    {
        return a;
    }
    return b;
}
int findMinTribe(Map tribes)
{
    Map temp=mapCopy(tribes);
    char* current_tribe=mapGetFirst(temp);
    int convert_current= atoi (current_tribe);
    int tribes_num=mapGetSize(temp);
    int min_tribe=convert_current;
    int i=0;
    for ( i=1 ; i<tribes_num ; i++)
    {
        char* next_tribe=mapGetNext(temp);
        int convert_next= atoi (next_tribe);
        min_tribe=minTribe(convert_next,min_tribe);
    }
    mapDestroy(temp);
    return min_tribe;
}

int GetMaxVotesTribe(Votes votes,int a_id , Map tribes)
{

    int max_votes=0,max_tribe_id=0;
    bool first_time=true;
    while (votes)
    {
        if (votes->area_id==a_id)
        {
            if (first_time)
            {
                max_tribe_id = votes->tribe_id;
                first_time = false;
                max_votes = votes->votes_num;

            }
            else if (votes->votes_num>max_votes){
                max_votes = votes->votes_num;
                max_tribe_id = votes->tribe_id;
            }
            else if(votes->votes_num == max_votes)
            {
                max_tribe_id = minTribe(max_tribe_id,votes->tribe_id);
            }
        }
        votes=votes->next;
    }
    if (max_votes==0)
    {
        return findMinTribe(tribes);
    }
    return max_tribe_id;
}

Votes voteReduce (Votes votes, int area_id, int tribe_id , int votes_num) {
    Votes temp = votes;
    while (votes) {

        if ((votes->area_id==area_id) && (votes->tribe_id== tribe_id))
        {
            votes->votes_num-=votes_num;
            if (votes->votes_num < 0)
            {
               votes->votes_num=0;
            }
            return temp;
        }
        votes=votes->next;
    }
    return temp;
}
