#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

namespace vox {
    namespace obj {

        class Texture {
            public:
                unsigned int textureId;
                unsigned int objRefs = 0;
                std::string filePath;
                Texture(std::string filePath);
                Texture();
            private:
                bool loadTexture();
        };

    }
}

#endif