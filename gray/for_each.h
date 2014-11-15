#include <cstddef>

namespace gray {
    template <typename Callback>
    void ForEach(size_t codeLength, Callback callback) {
        callback(-1);
        for (size_t i = 0x1; i < 1LL << codeLength; i++) {
            size_t x = i;
            int pos = 0;
            while (!(x & 0x1)) {
                x >>= 1;
                pos++;
            }
            callback(pos);
        }
    }
}
