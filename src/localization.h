#ifndef YAYA_LOCALIZATION_H
#define YAYA_LOCALIZATION_H

#include <string>

#define LANG_COUNT 2
#define LOC_ENTITIY_NAME 0
#define LOC_ENTITIY_DESC 1
#define LOC_ENTITY_CNT 2

const std::wstring strPickUpItem[LANG_COUNT] = {
        L"pick up item",
        L"拿起物品"
};
const std::wstring strHoldIn[LANG_COUNT][2] = {
        L"hold ", L" in ",
        L"將", L"拿在"
};
const std::wstring strDrop[LANG_COUNT][1] = {
        L"put down ",
        L"放下"
};
const std::wstring strDropItem[LANG_COUNT] = {
        L"drop item",
        L"放下物品"
};
const std::wstring strPickUpContainer[LANG_COUNT] = {
        L"pick up container",
        L"拿起容器"
};
const std::wstring strDropContainer[LANG_COUNT] = {
        L"drop container",
        L"放下容器"
};
const std::wstring strStoreItem[LANG_COUNT] = {
        L"store item",
        L"儲存物品"
};

const std::wstring strLeftHand[LANG_COUNT] = {
        L"left hand",
        L"左手"
};

const std::wstring strDuck[LOC_ENTITY_CNT][LANG_COUNT] = {
        L"the duck", L"鴨鴨",
        L"delicious, delicious", L"美味、可口"
};
const std::wstring strEgg[LOC_ENTITY_CNT][LANG_COUNT] = {
        L"the egg", L"蛋",
        L"a duck egg", L"鴨蛋，有可能會孵化，也有可能已經孵化了"
};
const std::wstring strEggCarton[LOC_ENTITY_CNT][LANG_COUNT] = {
        L"the egg carton", L"蛋盒",
        L"holds up to 10 eggs!", L"可以放十顆蛋！你沒聽錯，十顆！"
};

#endif //YAYA_LOCALIZATION_H
