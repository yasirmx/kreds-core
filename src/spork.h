
// Copyright (c) 2009-2012 The Kreds developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef SPORK_H
#define SPORK_H

//#include "bignum.h"
#include "sync.h"
#include "net.h"
#include "key.h"
#include "hash.h"
//#include "core.h"
#include "util.h"
#include "script/script.h"
#include "base58.h"
#include "net_processing.h"

using namespace std;
using namespace boost;

// Don't ever reuse these IDs for other sporks
#define SPORK_1_MASTERNODE_PAYMENTS_ENFORCEMENT               10000
#define SPORK_2_INSTANTX                                      10001
#define SPORK_3_INSTANTX_BLOCK_FILTERING                      10002
#define SPORK_4_NOTUSED                                       10003
#define SPORK_5_MAX_VALUE                                     10004
#define SPORK_6_NOTUSED                                       10005
#define SPORK_7_MASTERNODE_SCANNING                           10006

#define SPORK_1_MASTERNODE_PAYMENTS_ENFORCEMENT_DEFAULT       1424217600 // genesis but payments only after block 15000
#define SPORK_2_INSTANTX_DEFAULT                              978307200
#define SPORK_3_INSTANTX_BLOCK_FILTERING_DEFAULT              1424217600
#define SPORK_5_MAX_VALUE_DEFAULT                             10000
#define SPORK_7_MASTERNODE_SCANNING_DEFAULT                   978307200  

class CSporkMessage;
class CSporkManager;
class CProcessSpork;

//#include "bignum.h"
#include "net.h"
#include "key.h"
#include "util.h"
#include "protocol.h"
#include "sync.h"
#include "utilstrencodings.h"
//#include "darksend.h"
#include "validation.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

extern std::map<uint256, CSporkMessage> mapSporks;
extern std::map<int, CSporkMessage> mapSporksActive;
extern CSporkManager sporkManager;
extern CProcessSpork spMessage;

//void ProcessSpork(CNode* pfrom, const string& strCommand, CDataStream& vRecv, CConnman& connman);
int GetSporkValue(int nSporkID);
bool IsSporkActive(int nSporkID);
void ExecuteSpork(int nSporkID, int nValue);


void ProcessSpork(CNode* pfrom, const string& strCommand, CDataStream& vRecv, CConnman& connman);
//
// Spork Class
// Keeps track of all of the network spork settings
//

class CSporkMessage
{
public:
    std::vector<unsigned char> vchSig;
    int nSporkID;
    int64_t nValue;
    int64_t nTimeSigned;
    

     uint256 GetHash()
	{ 
    uint256 n;

		n = Hash(BEGIN(nSporkID), END(nTimeSigned));
		return n;
      
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action)
	{
    READWRITE(nSporkID);
    READWRITE(nValue);
    READWRITE(nTimeSigned);
    READWRITE(vchSig);
    }
};


class CSporkManager
{
private:
    std::vector<unsigned char> vchSig;

    std::string strMasterPrivKey;
    std::string strTestPubKey;
    std::string strMainPubKey;

public:

    CSporkManager() {
        
    // 100: G=0 101: MK just test
        strMainPubKey = "04d073216cf3a1293a3fd7d2e72ba005416122c93193812bb92b8a7846420c7aa941327b062ee00b0d1d31cdeaa26a740ae4e962ecd1a8f71bb8ebdc14035e6fa6";
        strTestPubKey = "043073216cf3a1293a3fd7d2e72ba005416122c93193812bb92b8a7846420c7aa941327b062ee00b0d1d31cdeaa26a740ae4e962ecd1a8f71bb8ebdc14035e6fa6";
    }

    std::string GetSporkNameByID(int id);
    int GetSporkIDByName(std::string strName);
    bool UpdateSpork(int nSporkID, int64_t nValue);
    bool SetPrivKey(std::string strPrivKey);
    bool CheckSignature(CSporkMessage& spork);
    bool Sign(CSporkMessage& spork);
    void Relay(CSporkMessage& msg, CConnman& connman);
	void RelayUpdateSpork(CSporkMessage& msg);

};

#endif