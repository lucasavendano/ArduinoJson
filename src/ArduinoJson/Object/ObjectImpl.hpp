// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/ObjectData.hpp>
#include <ArduinoJson/Variant/VariantCompare.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TAdaptedString>
inline VariantData* ObjectData::getMember(
    TAdaptedString key, const ResourceManager* resources) const {
  auto it = findKey(key, resources);
  if (it.done())
    return nullptr;
  it.next(resources);
  return it.data();
}

template <typename TAdaptedString>
VariantData* ObjectData::getOrAddMember(TAdaptedString key,
                                        ResourceManager* resources) {
  auto data = getMember(key, resources);
  if (data)
    return data;
  return addMember(key, resources);
}

template <typename TAdaptedString>
inline ObjectData::iterator ObjectData::findKey(
    TAdaptedString key, const ResourceManager* resources) const {
  if (key.isNull())
    return iterator();
  bool isKey = true;
  for (auto it = createIterator(resources); !it.done(); it.next(resources)) {
    if (isKey && stringEquals(key, adaptString(it->asString())))
      return it;
    isKey = !isKey;
  }
  return iterator();
}

template <typename TAdaptedString>
inline void ObjectData::removeMember(TAdaptedString key,
                                     ResourceManager* resources) {
  remove(findKey(key, resources), resources);
}

template <typename TAdaptedString>
inline VariantData* ObjectData::addMember(TAdaptedString key,
                                          ResourceManager* resources) {
  auto keySlot = resources->allocSlot();
  if (!keySlot)
    return nullptr;

  auto valueSlot = resources->allocSlot();
  if (!valueSlot)
    return nullptr;

  if (!keySlot->data()->setString(key, resources))
    return nullptr;

  CollectionData::appendPair(keySlot, valueSlot, resources);

  return valueSlot->data();
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
