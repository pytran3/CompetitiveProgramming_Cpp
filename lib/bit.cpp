int countBit(ull v) {
  // bitの数を数える
  ull count = (v & 0x5555555555555555) + ((v >> 1) & 0x5555555555555555);
  count = (count & 0x3333333333333333) + ((count >> 2) & 0x3333333333333333);
  count = (count & 0x0f0f0f0f0f0f0f0f) + ((count >> 4) & 0x0f0f0f0f0f0f0f0f);
  count = (count & 0x00ff00ff00ff00ff) + ((count >> 8) & 0x00ff00ff00ff00ff);
  count = (count & 0x0000ffff0000ffff) + ((count >> 16) & 0x0000ffff0000ffff);
  return (int)((count & 0x00000000ffffffff) + ((count >> 32) & 0x00000000ffffffff));
}

int padMSBRight(ull v) {
  // MSB以下を1埋め
  if (v == 0) return 0;
  v |= (v >> 1);
  v |= (v >> 2);
  v |= (v >> 4);
  v |= (v >> 8);
  v |= (v >> 16);
  v |= (v >> 32);
  return v;
}

int getMSB(ull v) {
  // 最上位bitの取得
  return countBit(padMSBRight(v)) - 1;
}

int padLSBLeft(ull v) {
  // LSB以上を0埋め
  if (v == 0) return 0;
  v |= (v << 1);
  v |= (v << 2);
  v |= (v << 4);
  v |= (v << 8);
  v |= (v << 16);
  v |= (v << 32);
  return v;
}

int getLSB(ull v) {
  // 最下位bitを取得
  return 64 - countBit(padLSBLeft(v));
}
