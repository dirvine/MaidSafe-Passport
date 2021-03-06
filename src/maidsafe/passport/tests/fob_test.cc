/*  Copyright 2012 MaidSafe.net limited

    This MaidSafe Software is licensed to you under (1) the MaidSafe.net Commercial License,
    version 1.0 or later, or (2) The General Public License (GPL), version 3, depending on which
    licence you accepted on initial access to the Software (the "Licences").

    By contributing code to the MaidSafe Software, or to this project generally, you agree to be
    bound by the terms of the MaidSafe Contributor Agreement, version 1.0, found in the root
    directory of this project at LICENSE, COPYING and CONTRIBUTOR respectively and also
    available at: http://www.maidsafe.net/licenses

    Unless required by applicable law or agreed to in writing, the MaidSafe Software distributed
    under the GPL Licence is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS
    OF ANY KIND, either express or implied.

    See the Licences for the specific language governing permissions and limitations relating to
    use of the MaidSafe Software.                                                                 */

#include "maidsafe/passport/detail/fob.h"

#include <string>

#include "maidsafe/common/log.h"
#include "maidsafe/common/rsa.h"
#include "maidsafe/common/test.h"
#include "maidsafe/common/utils.h"

#include "maidsafe/passport/detail/passport.pb.h"
#include "maidsafe/passport/types.h"

namespace maidsafe {

namespace passport {

namespace test {

TEST_CASE("Fob generation and validation", "[Fob][Behavioural]") {
  Anmaid anmaid;
  Maid maid(anmaid);
  Anpmid anpmid;
  Pmid pmid(anpmid);
  Anmpid anmpid;
  Mpid mpid(NonEmptyString(RandomAlphaNumericString(1 + RandomUint32() % 100)), anmpid);

  Anmaid anmaid1(anmaid);
  Maid maid1(maid);
  Anpmid anpmid1;
  Pmid pmid1(anpmid1);
  Anmpid anmpid1(anmpid);
  Mpid mpid1(mpid);

  Anmaid anmaid2(std::move(anmaid1));
  Maid maid2(std::move(maid1));
  Anpmid anpmid2(std::move(anpmid1));
  Pmid pmid2(std::move(pmid1));
  Anmpid anmpid2(std::move(anmpid1));
  Mpid mpid2(std::move(mpid1));

  anmaid1 = anmaid;
  maid1 = maid;
  anpmid1 = anpmid;
  pmid1 = pmid;
  anmpid1 = anmpid;
  mpid1 = mpid;

  anmaid2 = std::move(anmaid1);
  maid2 = std::move(maid1);
  anpmid2 = std::move(anpmid1);
  pmid2 = std::move(pmid1);
  anmpid2 = std::move(anmpid1);
  mpid2 = std::move(mpid1);

  static_assert(!is_short_term_cacheable<Anmaid>::value, "");
  static_assert(!is_short_term_cacheable<Maid>::value, "");
  static_assert(!is_short_term_cacheable<Anpmid>::value, "");
  static_assert(!is_short_term_cacheable<Pmid>::value, "");
  static_assert(!is_short_term_cacheable<Anmpid>::value, "");
  static_assert(!is_short_term_cacheable<Mpid>::value, "");
  static_assert(!is_long_term_cacheable<Anmaid>::value, "");
  static_assert(!is_long_term_cacheable<Maid>::value, "");
  static_assert(!is_long_term_cacheable<Anpmid>::value, "");
  static_assert(!is_long_term_cacheable<Pmid>::value, "");
  static_assert(!is_long_term_cacheable<Anmpid>::value, "");
  static_assert(!is_long_term_cacheable<Mpid>::value, "");
  CHECK(true);  // To avoid Catch '--warn NoAssertions' triggering a CTest failure.
}

template <typename Fobtype>
bool CheckSerialisationAndParsing(Fobtype fob) {
  maidsafe::passport::detail::protobuf::Fob proto_fob;
  fob.ToProtobuf(&proto_fob);
  Fobtype fob2(proto_fob);
  if (fob.validation_token() != fob2.validation_token()) {
    LOG(kError) << "Validation tokens don't match.";
    return false;
  }
  if (!asymm::MatchingKeys(fob.private_key(), fob2.private_key())) {
    LOG(kError) << "Private keys don't match.";
    return false;
  }
  if (!asymm::MatchingKeys(fob.public_key(), fob2.public_key())) {
    LOG(kError) << "Public keys don't match.";
    return false;
  }
  if (fob.name() != fob2.name()) {
    LOG(kError) << "Names don't match.";
    return false;
  }
  return true;
}

TEST_CASE("Fob serialisation and parsing", "[Fob][Behavioural]") {
  Anmaid anmaid;
  Maid maid(anmaid);
  Anpmid anpmid;
  Pmid pmid(anpmid);
  Anmpid anmpid;
  Mpid mpid(NonEmptyString(RandomAlphaNumericString(1 + RandomUint32() % 100)), anmpid);

  CHECK(CheckSerialisationAndParsing(anmaid));
  CHECK(CheckSerialisationAndParsing(maid));
  CHECK(CheckSerialisationAndParsing(anpmid));
  CHECK(CheckSerialisationAndParsing(pmid));
  CHECK(CheckSerialisationAndParsing(anmpid));
  CHECK(CheckSerialisationAndParsing(mpid));
}

bool CheckTokenAndName(const asymm::PublicKey& public_key, const asymm::Signature& signature,
                       const asymm::PublicKey& signer_key, const Identity& name,
                       NonEmptyString chosen_name = NonEmptyString()) {
  bool validation_result(
      asymm::CheckSignature(asymm::PlainText(asymm::EncodeKey(public_key)), signature, signer_key));
  if (!validation_result) {
    LOG(kError) << "Bad validation token.";
    return false;
  }

  Identity name_result;
  if (chosen_name.IsInitialised())
    name_result = crypto::Hash<crypto::SHA512>(chosen_name);
  else
    name_result = crypto::Hash<crypto::SHA512>(asymm::EncodeKey(public_key) + signature);
  if (name_result != name) {
    LOG(kError) << "Bad name.";
    return false;
  }
  return true;
}

template <typename Fobtype>
bool CheckNamingAndValidation(Fobtype fob) {
  return CheckTokenAndName(fob.public_key(), fob.validation_token(), fob.private_key(),
                           Identity(fob.name()));
}

template <typename Fobtype>
bool CheckNamingAndValidation(Fobtype fob, asymm::PublicKey signer_public_key,
                              NonEmptyString chosen_name = NonEmptyString()) {
  return CheckTokenAndName(fob.public_key(), fob.validation_token(), signer_public_key,
                           Identity(fob.name()), chosen_name);
}

TEST_CASE("Fob naming and validation", "[Fob][Behavioural]") {
  Anmaid anmaid;
  Maid maid(anmaid);
  Anpmid anpmid;
  Pmid pmid(anpmid);
  Anmpid anmpid;
  NonEmptyString chosen_name(RandomAlphaNumericString(1 + RandomUint32() % 100));
  Mpid mpid(chosen_name, anmpid);

  CHECK(CheckNamingAndValidation(anmaid));
  CHECK(CheckNamingAndValidation(maid, anmaid.public_key()));
  CHECK(CheckNamingAndValidation(anpmid));
  CHECK(CheckNamingAndValidation(pmid, anpmid.public_key()));
  CHECK(CheckNamingAndValidation(anmpid));
  CHECK(CheckNamingAndValidation(mpid, anmpid.public_key(), chosen_name));
}

}  // namespace test

}  // namespace passport

}  // namespace maidsafe
