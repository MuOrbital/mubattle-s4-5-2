#pragma once

// Season 5.2 MuHelper protocol bridge.  Keeping this outside WSclientinline.h
// avoids changing the legacy ANSI protocol header while retaining the exact
// packet contract used by the GameServer Helper module.
void SendRequestStartHelper(BYTE status);
void SendRequestSaveHelperSetting(BYTE* buffer, int size);
void ReceiveMuHelperData(BYTE* buffer);
void ReceiveMuHelperResult(BYTE* buffer);
void BindMuHelperCharacter(CHARACTER* character);
