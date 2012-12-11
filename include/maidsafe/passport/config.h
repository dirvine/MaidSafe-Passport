/*
* ============================================================================
*
* Copyright [2009] maidsafe.net limited
*
* Description:  Definition of error codes, typedef, forward declarations, etc.
* Version:      1.0
* Created:      2009-10-12-13.48.44
* Revision:     none
* Author:       Fraser Hutchison (fh), fraser.hutchison@maidsafe.net
* Company:      maidsafe.net limited
*
* The following source code is property of maidsafe.net limited and is not
* meant for external use.  The use of this code is governed by the license
* file LICENSE.TXT found in the root of this directory and also on
* www.maidsafe.net.
*
* You are not free to copy, amend or otherwise use this source code without
* the explicit written permission of the board of directors of maidsafe.net.
*
* ============================================================================
*/

#ifndef MAIDSAFE_PASSPORT_CONFIG_H_
#define MAIDSAFE_PASSPORT_CONFIG_H_

#include <tuple>

#include <string>
#include <vector>

#include "maidsafe/common/rsa.h"


namespace maidsafe {

namespace passport {

enum ReturnCode {
  kSuccess = 0,
  kPassportError = -100001,
  kNoPendingPacket = -100002,
  kNoPacket = -100003,
  kBadSerialisedKeyChain = -100004,
  kKeyChainNotEmpty = -100005,
  kPacketsNotEqual = -100006,
  kMissingDependentPackets = -100007,
  kNullPointer = -100008,
  kEmptyParameter = -100009,
  kFailedToCreatePacket = -100010,
  kFailedToConfirmPacket = -100011,
  kFailedToRevertPacket = -100012,
  kFailedToFindSelectableIdentity = -100013,
  kFailedToAddSelectableIdentity = -100014,
  kFailedToConfirmSelectableIdentity = -100015,
  kFailedToDeleteSelectableIdentity = -100016,
  kFailedToGetSelectableIdentityData = -100017
};

struct AnmidTag;
struct AnsmidTag;
struct AntmidTag;
struct AnmaidTag;
struct MaidTag;
struct PmidTag;
struct MidTag;
struct SmidTag;
struct TmidTag;
struct StmidTag;
struct AnmpidTag;
struct MpidTag;
struct MmidTag;

typedef TaggedValue<Identity, struct AnmidTag> AnmidName;
typedef TaggedValue<Identity, struct AnsmidTag> AnsmidName;
typedef TaggedValue<Identity, struct AntmidTag> AntmidName;
typedef TaggedValue<Identity, struct AnmaidTag> AnmaidName;
typedef TaggedValue<Identity, struct MaidTag> MaidName;
typedef TaggedValue<Identity, struct PmidTag> PmidName;
typedef TaggedValue<Identity, struct MidTag> MidName;
typedef TaggedValue<Identity, struct SmidTag> SmidName;
typedef TaggedValue<Identity, struct TmidTag> TmidName;
typedef TaggedValue<Identity, struct StmidTag> StmidName;
typedef TaggedValue<Identity, struct AnmpidTag> AnmpidName;
typedef TaggedValue<Identity, struct MpidTag> MpidName;
typedef TaggedValue<Identity, struct MmidTag> MmidName;

}  // namespace passport

class PmidFob {
  // dummy class //TODO(dirvine) delete me
};

}  // namespace maidsafe

#endif  // MAIDSAFE_PASSPORT_CONFIG_H_