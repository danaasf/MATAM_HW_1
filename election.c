//
// Created by danaa on 5/1/2020.
//
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <stdbool.h>
#include "election.h"
#include "mtm_map/map.h"
#include "votes.h"
#define DIVIDER 10
#define ONE_DIGIT 1

struct election_t {
    Map areas;
    Map tribes;
    Votes votes;
};

Election electionCreate()
{
    Election system=malloc(sizeof(*system));
    if (!system)
    {
        return NULL;
    }
    if ((system->areas=mapCreate())==NULL||(system->tribes=mapCreate())==NULL ||(system->votes=votesCreate())==NULL)
    {
        mapDestroy(system->areas);
        mapDestroy(system->tribes);
        votesDestroy(system->votes);
        return NULL;
    }
    return system;
}

void electionDestroy(Election election)
{
    if (!election) {
        return;
    }
    mapDestroy(election->areas);
    mapDestroy(election->tribes);
    votesDestroy(election->votes);

    free(election);
}

static bool nameCheck (const char* tribe_name)
{
    while (*tribe_name)
    {
        if (((*tribe_name > 'z') || (*tribe_name < 'a')) && (*tribe_name != ' '))
        {
            return false;
        }
        tribe_name++;
    }
    return true;
}

static int numberOfDigits (int number)
{
    int digits=0;
    if (number==0)
    {
        return ONE_DIGIT ;
    }
    while (number>0)
    {
        number/= DIVIDER;
        digits++;
    }
    return digits;
}

static char* intToString (int number)
{
    int digits=numberOfDigits(number);
    char *str=malloc(digits+1);
    if (!(str))
    {
        return NULL;
    }
    sprintf(str,"%d",number);
    return str;
}

ElectionResult electionAddTribe (Election election, int tribe_id, const char* tribe_name)
{
    if((election==NULL) || (tribe_name==NULL) )
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id<0)
    {
        return ELECTION_INVALID_ID;
    }
    if (nameCheck(tribe_name) == false)
    {
        return ELECTION_INVALID_NAME;
    }
    char* id=intToString(tribe_id);
    if (mapContains(election->tribes,id)) {
        free(id);
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    char* name= malloc(strlen(tribe_name)+1);
    if (name==NULL){
        free(id);
        free(name);
        return ELECTION_OUT_OF_MEMORY;
    }
    strcpy(name,tribe_name);
    MapResult result_check  = mapPut(election->tribes,id,name);
    if (result_check== MAP_OUT_OF_MEMORY)
    {
        electionDestroy(election);
        free(name);
        free(id);
        return ELECTION_OUT_OF_MEMORY;
    }
    free(name);
    free(id);
    return ELECTION_SUCCESS;
}

ElectionResult electionAddArea(Election election, int area_id, const char* area_name) {
    if((election==NULL) || (area_name==NULL) )
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if (area_id<0)
    {
        return ELECTION_INVALID_ID;
    }
    if (nameCheck(area_name) == false)
    {
        return ELECTION_INVALID_NAME;
    }
    char* id=intToString(area_id);
    if (mapContains(election->areas,id)) {
        free(id);
        return ELECTION_AREA_ALREADY_EXIST;
    }
    MapResult result_check= mapPut(election->areas,id,area_name);
    if ( result_check == MAP_OUT_OF_MEMORY)
    {
        free(id);
        return ELECTION_OUT_OF_MEMORY;
    }
    free(id);
    return ELECTION_SUCCESS;
}

char* electionGetTribeName (Election election, int tribe_id) {
    if ((election==NULL)  || (tribe_id<0) ) {
        return NULL;
    }
    char* id=intToString(tribe_id);
    char* name= mapGet(election->tribes,id);
    if (name==NULL)
    {
        free(id);
        return NULL;
    }
    char *value= malloc(sizeof(char)*(strlen(name)+1));
    if (value==NULL)
    {
        free(id);
        free(value);
        return NULL;
    }
    strcpy(value,name);
    free(id);
    return value ;
}

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name) {
    if ((election==NULL) || (tribe_name==NULL))
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id<0)
    {
        return ELECTION_INVALID_ID;
    }

    if (!(nameCheck(tribe_name))){
        return ELECTION_INVALID_NAME;
    }
    char* id=intToString(tribe_id);
    if (!(mapContains(election->tribes,id)))
    {
        free(id);
        return ELECTION_TRIBE_NOT_EXIST;
    }
    MapResult result =mapPut(election->tribes,id,tribe_name);
    if (result == MAP_SUCCESS)
    {
        free(id);
        return ELECTION_SUCCESS;
    }
    else if (result== MAP_OUT_OF_MEMORY)
    {
        free(id);
        return ELECTION_OUT_OF_MEMORY;
    }
    free(id);
    return ELECTION_NULL_ARGUMENT;

}

ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes) {
    if (election==NULL)
    {
        return ELECTION_NULL_ARGUMENT;

    }
    if ((area_id<0) || (tribe_id<0))
    {
        return ELECTION_INVALID_ID;
    }
    if (num_of_votes<=0)
    {
        return ELECTION_INVALID_VOTES;
    }

    char *t_id=  intToString(tribe_id);
    char *a_id=intToString(area_id);
    if (!(mapContains(election->areas,a_id)))
    {
        free(t_id);
        free(a_id);
        return ELECTION_AREA_NOT_EXIST;
    }

    if (!(mapContains(election->tribes,t_id)))
    {
        free(t_id);
        free(a_id);
        return ELECTION_TRIBE_NOT_EXIST;
    }
    Votes temp = election->votes;
    while(temp)
    {
            if ((election->votes->area_id==area_id) && (election->votes->tribe_id== tribe_id))
            {
                election->votes->votes_num+=num_of_votes;
                free(t_id);
                free(a_id);
                return ELECTION_SUCCESS;
            }
            temp = temp->next;
    }
    election->votes=votePut(election->votes,area_id,tribe_id,num_of_votes);
    if (election->votes)
    {
        free(t_id);
        free(a_id);
        return ELECTION_SUCCESS;
    }
    free(t_id);
    free(a_id);
    return ELECTION_OUT_OF_MEMORY;
}

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes){
    if (election==NULL)
    {
        return ELECTION_NULL_ARGUMENT;

    }
    if ((area_id<0) || (tribe_id<0))
    {
        return ELECTION_INVALID_ID;
    }
    if (num_of_votes<=0)
    {
        return ELECTION_INVALID_VOTES;
    }
    char *t_id=  intToString(tribe_id);
    char *a_id=intToString(area_id);

    if (!(mapContains(election->areas,a_id)))
    {
        free(t_id);
        free(a_id);
        return ELECTION_AREA_NOT_EXIST;
    }

    if (!(mapContains(election->tribes,t_id)))
    {
            free(t_id);
            free(a_id);
            return ELECTION_TRIBE_NOT_EXIST;
    }
    election->votes=voteReduce(election->votes,area_id,tribe_id,num_of_votes);
    free(t_id);
    free(a_id);
    return ELECTION_SUCCESS;
}


ElectionResult electionRemoveTribe (Election election, int tribe_id){
    if (election==NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id<0)
    {
        return ELECTION_INVALID_ID;
    }
    char* id=intToString(tribe_id);
    if (!(mapContains(election->tribes,id)))
    {
        free(id);
        return ELECTION_TRIBE_NOT_EXIST;
    }
    election->votes= removeVotesByTribe(election->votes,tribe_id);
    MapResult result= mapRemove(election->tribes,id);
   if (result== MAP_SUCCESS) {
       free(id);
       return ELECTION_SUCCESS;
   }
   free(id);
   return ELECTION_ERROR;
}

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area) {
    if (election==NULL || should_delete_area==NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    char* current_id = mapGetFirst(election->areas);
    while(current_id)
    {
        int a_id = atoi(current_id);
        if (should_delete_area(a_id))
        {
           election->votes= removeVotesByArea(election->votes,a_id);
            MapResult result = mapRemove(election->areas,current_id);
            if (result != MAP_SUCCESS)
            {
                return ELECTION_ERROR;
            }
            current_id = mapGetFirst(election->areas);
            continue;
        }
        current_id= mapGetNext(election->areas);
    }

    return ELECTION_SUCCESS;
}


Map electionComputeAreasToTribesMapping (Election election) {
    Map area_mapping=mapCreate();
    if (!(area_mapping)) {
        return NULL;
    }
    if (!(election))
    {
        return NULL;
    }

    char* current_area = mapGetFirst(election->areas);
    int max_tribe=0 ;
    int areas_num=mapGetSize(election->areas);
    int i=0;
    for (i=0; i< areas_num ; i++)
    {
        int a_id = atoi(current_area);
        max_tribe = GetMaxVotesTribe(election->votes,a_id,election->tribes);
        char *tribe_id = intToString(max_tribe);
        mapPut(area_mapping,current_area,tribe_id);
        current_area = mapGetNext(election->areas);
        free(tribe_id);
    }

    return area_mapping;
}

