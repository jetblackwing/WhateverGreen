//
//  kern_igfx_kexts.cpp
//  WhateverGreen
//
//  Created by FireWolf on 8/6/21.
//  Copyright © 2021 vit9696. All rights reserved.
//

#include "kern_igfx_kexts.hpp"

static const char *pathIntelICL[]     { "/System/Library/Extensions/AppleIntelICLGraphics.kext/Contents/MacOS/AppleIntelICLGraphics" };
static const char *pathIntelICLLPFb[] { "/System/Library/Extensions/AppleIntelICLLPGraphicsFramebuffer.kext/Contents/MacOS/AppleIntelICLLPGraphicsFramebuffer" };
static const char *pathIntelICLHPFb[] { "/System/Library/Extensions/AppleIntelICLHPGraphicsFramebuffer.kext/Contents/MacOS/AppleIntelICLHPGraphicsFramebuffer" };
static const char *pathIntelTGL[]     { "/System/Library/Extensions/AppleIntelTGLGraphics.kext/Contents/MacOS/AppleIntelTGLGraphics"};
static const char *pathIntelTGLLPFb[] { "/System/Library/Extensions/AppleIntelTGLLPGraphicsFramebuffer.kext/Contents/MacOS/AppleIntelTGLLPGraphicsFramebuffer" };
static const char *pathIntelTGLHPFb[] { "/System/Library/Extensions/AppleIntelTGLHPGraphicsFramebuffer.kext/Contents/MacOS/AppleIntelTGLHPGraphicsFramebuffer" };

KernelPatcher::KextInfo kextIntelICL     { "com.apple.driver.AppleIntelICLGraphics", pathIntelICL, arrsize(pathIntelICL), {}, {}, KernelPatcher::KextInfo::Unloaded };
KernelPatcher::KextInfo kextIntelICLLPFb { "com.apple.driver.AppleIntelICLLPGraphicsFramebuffer", pathIntelICLLPFb, arrsize(pathIntelICLLPFb), {}, {}, KernelPatcher::KextInfo::Unloaded };
KernelPatcher::KextInfo kextIntelICLHPFb { "com.apple.driver.AppleIntelICLHPGraphicsFramebuffer", pathIntelICLHPFb, arrsize(pathIntelICLHPFb), {}, {}, KernelPatcher::KextInfo::Unloaded };
KernelPatcher::KextInfo kextIntelTGL     { "com.apple.driver.AppleIntelTGLGraphics", pathIntelTGL, arrsize(pathIntelTGL), {}, {}, KernelPatcher::KextInfo::Unloaded };
KernelPatcher::KextInfo kextIntelTGLLPFb { "com.apple.driver.AppleIntelTGLLPGraphicsFramebuffer", pathIntelTGLLPFb, arrsize(pathIntelTGLLPFb), {}, {}, KernelPatcher::KextInfo::Unloaded };
KernelPatcher::KextInfo kextIntelTGLHPFb { "com.apple.driver.AppleIntelTGLHPGraphicsFramebuffer", pathIntelTGLHPFb, arrsize(pathIntelTGLHPFb), {}, {}, KernelPatcher::KextInfo::Unloaded };
