/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include "TextureManager.h"

#include <gtest/gtest.h>

using namespace WebCore;
using namespace WTF;

namespace {

class FakeTextureAllocator : public TextureAllocator {
public:
    virtual unsigned createTexture(const IntSize&, GC3Denum) { return 1; }
    virtual void deleteTexture(unsigned, const IntSize&, GC3Denum) { }
};

FakeTextureAllocator fakeTextureAllocator;
const IntSize textureSize(256, 256);
const GC3Denum textureFormat = GraphicsContext3D::RGBA;

size_t texturesMemorySize(size_t textureCount)
{
    return TextureManager::memoryUseBytes(textureSize, textureFormat) * textureCount;
}

PassOwnPtr<TextureManager> createTextureManager(size_t maxTextures, size_t preferredTextures)
{
    return TextureManager::create(texturesMemorySize(maxTextures), texturesMemorySize(preferredTextures), 1024);
}

bool requestTexture(TextureManager* manager, TextureToken token)
{
    unsigned textureId;
    bool result = manager->requestTexture(token, textureSize, textureFormat, textureId);
    if (result)
        manager->allocateTexture(&fakeTextureAllocator, token);
    return result;
}

TEST(TextureManagerTest, requestTextureInPreferredLimit)
{
    const size_t preferredTextures = 8;
    OwnPtr<TextureManager> textureManager = createTextureManager(preferredTextures * 2, preferredTextures);
    TextureToken tokens[preferredTextures];
    for (size_t i = 0; i < preferredTextures; ++i) {
        tokens[i] = textureManager->getToken();
        if (i)
            EXPECT_GT(tokens[i], tokens[i - 1]);
        EXPECT_TRUE(requestTexture(textureManager.get(), tokens[i]));
        EXPECT_TRUE(textureManager->hasTexture(tokens[i]));
        EXPECT_TRUE(textureManager->isProtected(tokens[i]));
    }

    for (size_t i = 0; i < preferredTextures; ++i)
        EXPECT_TRUE(textureManager->hasTexture(tokens[i]));

    EXPECT_EQ(texturesMemorySize(preferredTextures), textureManager->currentMemoryUseBytes());
}

TEST(TextureManagerTest, requestTextureExceedingPreferredLimit)
{
    const size_t maxTextures = 8;
    const size_t preferredTextures = 4;
    OwnPtr<TextureManager> textureManager = createTextureManager(maxTextures, preferredTextures);
    TextureToken tokens[maxTextures];
    for (size_t i = 0; i < preferredTextures; ++i) {
        tokens[i] = textureManager->getToken();
        EXPECT_TRUE(requestTexture(textureManager.get(), tokens[i]));
        EXPECT_TRUE(textureManager->hasTexture(tokens[i]));
    }

    textureManager->unprotectTexture(tokens[0]);
    textureManager->unprotectTexture(tokens[2]);

    for (size_t i = preferredTextures; i < maxTextures; ++i) {
        tokens[i] = textureManager->getToken();
        EXPECT_TRUE(requestTexture(textureManager.get(), tokens[i]));
        EXPECT_TRUE(textureManager->hasTexture(tokens[i]));
        textureManager->unprotectTexture(tokens[i]);
    }

    EXPECT_FALSE(textureManager->hasTexture(tokens[0]));
    EXPECT_TRUE(textureManager->hasTexture(tokens[1]));
    EXPECT_TRUE(textureManager->isProtected(tokens[1]));
    EXPECT_FALSE(textureManager->hasTexture(tokens[2]));
    EXPECT_TRUE(textureManager->hasTexture(tokens[3]));
    EXPECT_TRUE(textureManager->isProtected(tokens[3]));

    EXPECT_EQ(texturesMemorySize(preferredTextures), textureManager->currentMemoryUseBytes());
}

TEST(TextureManagerTest, requestTextureExceedingMaxLimit)
{
    const size_t maxTextures = 8;
    const size_t preferredTextures = 4;
    OwnPtr<TextureManager> textureManager = createTextureManager(maxTextures, preferredTextures);
    TextureToken tokens[maxTextures];
    for (size_t i = 0; i < maxTextures; ++i) {
        tokens[i] = textureManager->getToken();
        EXPECT_TRUE(requestTexture(textureManager.get(), tokens[i]));
        EXPECT_TRUE(textureManager->hasTexture(tokens[i]));
    }

    EXPECT_EQ(texturesMemorySize(maxTextures), textureManager->currentMemoryUseBytes());

    for (size_t i = 0; i < maxTextures; ++i) {
        TextureToken token = textureManager->getToken();
        EXPECT_FALSE(requestTexture(textureManager.get(), token));
        EXPECT_FALSE(textureManager->hasTexture(token));
    }

    EXPECT_EQ(textureManager->currentMemoryUseBytes(), texturesMemorySize(maxTextures));

    textureManager->unprotectTexture(tokens[1]);
    textureManager->unprotectTexture(tokens[3]);
    EXPECT_TRUE(requestTexture(textureManager.get(), textureManager->getToken()));
    EXPECT_TRUE(requestTexture(textureManager.get(), textureManager->getToken()));
    EXPECT_FALSE(requestTexture(textureManager.get(), textureManager->getToken()));
    EXPECT_EQ(texturesMemorySize(maxTextures), textureManager->currentMemoryUseBytes());
    EXPECT_FALSE(textureManager->hasTexture(tokens[1]));
    EXPECT_FALSE(textureManager->hasTexture(tokens[3]));
}

TEST(TextureManagerTest, reduceMemoryToLimit)
{
    const size_t maxTextures = 8;
    const size_t preferredTextures = 4;
    OwnPtr<TextureManager> textureManager = createTextureManager(maxTextures, preferredTextures);
    TextureToken tokens[maxTextures];
    for (size_t i = 0; i < maxTextures; ++i) {
        tokens[i] = textureManager->getToken();
        EXPECT_TRUE(requestTexture(textureManager.get(), tokens[i]));
    }

    EXPECT_EQ(texturesMemorySize(maxTextures), textureManager->currentMemoryUseBytes());
    textureManager->reduceMemoryToLimit(texturesMemorySize(maxTextures));
    EXPECT_EQ(texturesMemorySize(maxTextures), textureManager->currentMemoryUseBytes());
    textureManager->reduceMemoryToLimit(texturesMemorySize(preferredTextures));
    EXPECT_EQ(texturesMemorySize(maxTextures), textureManager->currentMemoryUseBytes());

    const size_t unprotectedTextures = preferredTextures + 1;
    for (size_t i = 0; i < preferredTextures + 1; ++i)
        textureManager->unprotectTexture(tokens[i]);

    textureManager->reduceMemoryToLimit(texturesMemorySize(maxTextures));
    EXPECT_EQ(texturesMemorySize(maxTextures), textureManager->currentMemoryUseBytes());
    textureManager->reduceMemoryToLimit(texturesMemorySize(preferredTextures));
    EXPECT_EQ(texturesMemorySize(preferredTextures), textureManager->currentMemoryUseBytes());
    textureManager->reduceMemoryToLimit(texturesMemorySize(1));
    EXPECT_EQ(texturesMemorySize(maxTextures - unprotectedTextures), textureManager->currentMemoryUseBytes());

    // reduceMemoryToLimit doesn't change the current memory limits.
    EXPECT_EQ(texturesMemorySize(maxTextures), textureManager->maxMemoryLimitBytes());
    EXPECT_EQ(texturesMemorySize(preferredTextures), textureManager->preferredMemoryLimitBytes());
}

TEST(TextureManagerTest, setMaxMemoryLimitBytes)
{
    const size_t maxTextures = 8;
    const size_t preferredTextures = 4;
    OwnPtr<TextureManager> textureManager = createTextureManager(maxTextures, preferredTextures);
    TextureToken tokens[maxTextures];
    for (size_t i = 0; i < maxTextures; ++i) {
        tokens[i] = textureManager->getToken();
        EXPECT_TRUE(requestTexture(textureManager.get(), tokens[i]));
    }

    EXPECT_EQ(texturesMemorySize(maxTextures), textureManager->currentMemoryUseBytes());

    const size_t unprotectedTextures = preferredTextures + 1;
    for (size_t i = 0; i < unprotectedTextures; ++i)
        textureManager->unprotectTexture(tokens[i]);

    textureManager->setMaxMemoryLimitBytes(texturesMemorySize(maxTextures));
    EXPECT_EQ(texturesMemorySize(maxTextures), textureManager->currentMemoryUseBytes());
    textureManager->setMaxMemoryLimitBytes(texturesMemorySize(preferredTextures));
    EXPECT_EQ(texturesMemorySize(preferredTextures), textureManager->currentMemoryUseBytes());
    EXPECT_EQ(texturesMemorySize(preferredTextures), textureManager->maxMemoryLimitBytes());
}

TEST(TextureManagerTest, setPreferredMemoryLimitBytes)
{
    const size_t maxTextures = 8;
    const size_t preferredTextures = 4;
    OwnPtr<TextureManager> textureManager = createTextureManager(maxTextures, preferredTextures);
    TextureToken tokens[maxTextures];
    for (size_t i = 0; i < maxTextures; ++i) {
        tokens[i] = textureManager->getToken();
        EXPECT_TRUE(requestTexture(textureManager.get(), tokens[i]));
    }

    const size_t unprotectedTextures = preferredTextures + 1;
    for (size_t i = 0; i < unprotectedTextures; ++i)
        textureManager->unprotectTexture(tokens[i]);

    EXPECT_EQ(texturesMemorySize(maxTextures), textureManager->currentMemoryUseBytes());
    EXPECT_EQ(texturesMemorySize(maxTextures), textureManager->maxMemoryLimitBytes());

    // Setting preferred memory limit only won't force reduceMemoryToLimit.
    textureManager->setPreferredMemoryLimitBytes(texturesMemorySize(preferredTextures));
    EXPECT_EQ(texturesMemorySize(maxTextures), textureManager->currentMemoryUseBytes());
    EXPECT_EQ(texturesMemorySize(maxTextures), textureManager->maxMemoryLimitBytes());
    EXPECT_EQ(texturesMemorySize(preferredTextures), textureManager->preferredMemoryLimitBytes());
}

} // namespace
