//
// Created by danaa on 5/11/2020.
//
#include <stdbool.h>
#include "mtm_map/map.h"
#ifndef EX1_VOTES_H
#define EX1_VOTES_H

typedef struct votes_t {
    int area_id;
    int tribe_id;
    int votes_num;
    struct votes_t* next;
}*Votes;

/**
* VotesCreate: Allocates a new vote.
*
* @return
* 	NULL - if allocations failed.
* 	A new vote in case of success.
*/
Votes votesCreate ();
/**
 * votesPut : places a new vote at the beginning of the votes ADT.
 * @param votes : the votes
 * @param area_id : the area of which the vote is taken
 * @param tribe_id : the tribe to which the vote is to
 * @param votes_num : the number of votes (from the specific area to the wanted tribe)
 * @return  the votes after the needed vote was places/put
 */

Votes votePut( Votes votes, int area_id , int tribe_id , int votes_num);
/**
* votesDestroy: Deallocates an existing vote. Clears all elements from the vote.
*
* @param vote- Target vote to be deallocated. If the vote is NULL nothing will be
* 		done
*/

void votesDestroy (Votes votes);

/** removeVotesByArea: removes the votes based on the given ID of a specific area.
 *
 * @param votes: the votes ADT
 * @param area_id: the area of which we want to get rid of the votes
 */

Votes removeVotesByArea (Votes votes, int area_id);
/** removeVotesByTribe: removes the votes based on the given ID of a specific tribe.
 *
 * @param votes: the votes ADT
 * @param tribe_id: the tribe of which we want to get rid of the votes
 */

Votes removeVotesByTribe (Votes votes, int tribe_id);

/** minTribe: Checks and detecs the tribe with the lower ID.
 * @param a : the ID of tribe a
 * @param b :the ID of tribe b
 * @return - the lower ID
 */
int minTribe ( int a , int b);

/** findMinTribe : finds the tribe to which there is minimum votes
 *
 * @param map tribes
 * @return the area ID of the minimum tribe
 * if two tribes have the same number we take the minimum of of them
 */
int findMinTribe(Map tribes);

/** GetMaxVotesTribe : finds the tribe to which the most voted
 *
 * @param votes : the votes
 * @param a_id : the ID of the tribe
 * @param tribes : the map of tribes
 * @return the tribe ID of the max tribe
 */
int GetMaxVotesTribe(Votes votes,int a_id, Map tribes);

/** voteReduce : reduces the votes by a number of votes based on the followning params:
 * @param votes
 * @param area_id : the area the vote is from
 * @param tribe_id : the tribe the vote is to
 * @param votes_num : the number of votes to reduce
 * @return the votes after removing these votes
 */
Votes voteReduce (Votes votes, int area_id, int tribe_id , int votes_num);
#endif //EX1_VOTES_H
