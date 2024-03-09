//
//  kern_model.cpp
//  WhateverGreen
//
//  Copyright © 2017 vit9696. All rights reserved.
//

#include <Headers/kern_iokit.hpp>

#include "kern_weg.hpp"

/**
 * General rules for AMD GPU names (first table):
 *
 * 1. Only use device identifiers present in Apple kexts starting with 5xxx series
 * 2. Follow Apple naming style (e.g., ATI for 5xxx series, AMD for 6xxx series, Radeon Pro for 5xx GPUs)
 * 3. Write earliest available hardware with slashes (e.g. Radeon Pro 455/555)
 * 4. Avoid using generic names like "AMD Radeon RX"
 * 5. Provide revision identifiers whenever possible
 * 6. Detection order should be vendor-id, device-id, subsystem-vendor-id, subsystem-id, revision-id
 *
 * General rules for Intel GPU names (second table):
 *
 * 1. All identifiers from Sandy and newer are allowed to be present
 * 2. For idenitifiers not present in Apple kexts provide a fake id (AMD GPUs are just too many)
 * 3. Use nullptr if the exact GPU name is not known
 *
 * Some identifiers are taken from https://github.com/pciutils/pciids/blob/master/pci.ids?raw=true
 * Last synced version 2017.07.24 (https://github.com/pciutils/pciids/blob/699e70f3de/pci.ids?raw=true)
 */

struct Model {
	enum Detect : uint16_t {
		DetectDef = 0x0,
		DetectSub = 0x1,
		DetectRev = 0x2,
		DetectAll = DetectSub | DetectRev
	};
	uint16_t mode {DetectDef};
	uint16_t subven {0};
	uint16_t sub {0};
	uint16_t rev {0};
	const char *name {nullptr};
};

struct BuiltinModel {
	uint32_t device;
	uint32_t fake;
	const char *name;
};

struct DevicePair {
	uint16_t dev;
	const Model *models;
	size_t modelNum;
};

static constexpr DevicePair devices[] {
	{0x6640, dev6640, arrsize(dev6640)},
	{0x6641, dev6641, arrsize(dev6641)},
	{0x6646, dev6646, arrsize(dev6646)},
	{0x6647, dev6647, arrsize(dev6647)},
	{0x665c, dev665c, arrsize(dev665c)},
	{0x665d, dev665d, arrsize(dev665d)},
	{0x66af, dev66af, arrsize(dev66af)},
	{0x6704, dev6704, arrsize(dev6704)},
	{0x6718, dev6718, arrsize(dev6718)},
	{0x6719, dev6719, arrsize(dev6719)},
	{0x6720, dev6720, arrsize(dev6720)},
	{0x6722, dev6722, arrsize(dev6722)},
	{0x6738, dev6738, arrsize(dev6738)},
	{0x6739, dev6739, arrsize(dev6739)},
	{0x6740, dev6740, arrsize(dev6740)},
	{0x6741, dev6741, arrsize(dev6741)},
	{0x6745, dev6745, arrsize(dev6745)},
	{0x6750, dev6750, arrsize(dev6750)},
	{0x6758, dev6758, arrsize(dev6758)},
	{0x6759, dev6759, arrsize(dev6759)},
	{0x6760, dev6760, arrsize(dev6760)},
	{0x6761, dev6761, arrsize(dev6761)},
	{0x6768, dev6768, arrsize(dev6768)},
	{0x6770, dev6770, arrsize(dev6770)},
	{0x6779, dev6779, arrsize(dev6779)},
	{0x6780, dev6780, arrsize(dev6780)},
	{0x6790, dev6790, arrsize(dev6790)},
	{0x6798, dev6798, arrsize(dev6798)},
	{0x679a, dev679a, arrsize(dev679a)},
	{0x679e, dev679e, arrsize(dev679e)},
	{0x67b0, dev67b0, arrsize(dev67b0)},
	{0x67c0, dev67c0, arrsize(dev67c0)},
	{0x67c4, dev67c4, arrsize(dev67c4)},
	{0x67c7, dev67c7, arrsize(dev67c7)},
	{0x67df, dev67df, arrsize(dev67df)},
	{0x67e0, dev67e0, arrsize(dev67e0)},
	{0x67e3, dev67e3, arrsize(dev67e3)},
	{0x67ef, dev67ef, arrsize(dev67ef)},
	{0x67ff, dev67ff, arrsize(dev67ff)},
	{0x6800, dev6800, arrsize(dev6800)},
	{0x6801, dev6801, arrsize(dev6801)},
	{0x6806, dev6806, arrsize(dev6806)},
	{0x6808, dev6808, arrsize(dev6808)},
	{0x6810, dev6810, arrsize(dev6810)},
	{0x6818, dev6818, arrsize(dev6818)},
	{0x6819, dev6819, arrsize(dev6819)},
	{0x6820, dev6820, arrsize(dev6820)},
	{0x6821, dev6821, arrsize(dev6821)},
	{0x6823, dev6823, arrsize(dev6823)},
	{0x6825, dev6825, arrsize(dev6825)},
	{0x6827, dev6827, arrsize(dev6827)},
	{0x682b, dev682b, arrsize(dev682b)},
	{0x682d, dev682d, arrsize(dev682d)},
	{0x682f, dev682f, arrsize(dev682f)},
	{0x6835, dev6835, arrsize(dev6835)},
	{0x6839, dev6839, arrsize(dev6839)},
	{0x683b, dev683b, arrsize(dev683b)},
	{0x683d, dev683d, arrsize(dev683d)},
	{0x683f, dev683f, arrsize(dev683f)},
	{0x6840, dev6840, arrsize(dev6840)},
	{0x6841, dev6841, arrsize(dev6841)},
	{0x6861, dev6861, arrsize(dev6861)},
	{0x6863, dev6863, arrsize(dev6863)},
	{0x6868, dev6868, arrsize(dev6868)},	
	{0x687f, dev687f, arrsize(dev687f)},
	{0x6898, dev6898, arrsize(dev6898)},
	{0x6899, dev6899, arrsize(dev6899)},
	{0x68a0, dev68a0, arrsize(dev68a0)},
	{0x68a1, dev68a1, arrsize(dev68a1)},
	{0x68b0, dev68b0, arrsize(dev68b0)},
	{0x68b1, dev68b1, arrsize(dev68b1)},
	{0x68b8, dev68b8, arrsize(dev68b8)},
	{0x68c0, dev68c0, arrsize(dev68c0)},
	{0x68c1, dev68c1, arrsize(dev68c1)},
	{0x68d8, dev68d8, arrsize(dev68d8)},
	{0x68d9, dev68d9, arrsize(dev68d9)},
	{0x68e0, dev68e0, arrsize(dev68e0)},
	{0x68e1, dev68e1, arrsize(dev68e1)},
	{0x6920, dev6920, arrsize(dev6920)},
	{0x6921, dev6921, arrsize(dev6921)},
	{0x6938, dev6938, arrsize(dev6938)},
	{0x6939, dev6939, arrsize(dev6939)},
	{0x7300, dev7300, arrsize(dev7300)},
	{0x944a, dev944a, arrsize(dev944a)},
	{0x9488, dev9488, arrsize(dev9488)}
};

static BuiltinModel devIntel[] {
	// For Sandy only 0x0116 and 0x0126 controllers are properly supported by AppleIntelSNBGraphicsFB.
	// 0x0102 and 0x0106 are implemented as AppleIntelSNBGraphicsController/AppleIntelSNBGraphicsController2.
	// AppleIntelHD3000Graphics actually supports more (0x0106, 0x0601, 0x0102, 0x0116, 0x0126).
	// To make sure we have at least acceleration we fake unsupported ones as 0x0102.
	// 0x0106 is likely a typo from 0x0106 or a fulty device (AppleIntelHD3000Graphics)
	{ 0x0106, 0x0000, "Intel HD Graphics 2000" },
	{ 0x0601, 0x0106, "Intel HD Graphics 2000" },
	{ 0x0102, 0x0000, "Intel HD Graphics 2000" },
	{ 0x0112, 0x0116, "Intel HD Graphics 2000" },
	{ 0x0116, 0x0000, "Intel HD Graphics 3000" },
	{ 0x0122, 0x0126, "Intel HD Graphics 2000" },
	{ 0x0126, 0x0000, "Intel HD Graphics 3000" },
	{ 0x0152, 0x0000, "Intel HD Graphics 2500" },
	{ 0x015A, 0x0152, "Intel HD Graphics P2500" },
	{ 0x0156, 0x0000, "Intel HD Graphics 2500" },
	{ 0x0162, 0x0000, "Intel HD Graphics 4000" },
	{ 0x016A, 0x0162, "Intel HD Graphics P4000" },
	{ 0x0166, 0x0000, "Intel HD Graphics 4000" },
	{ 0x0D26, 0x0000, "Intel Iris Pro Graphics 5200" },
	{ 0x0D22, 0x0000, "Intel Iris Pro Graphics 5200" },
	{ 0x0D2A, 0x0000, "Intel Iris Pro Graphics 5200" },
	{ 0x0D2B, 0x0000, "Intel Iris Pro Graphics 5200" },
	{ 0x0D2E, 0x0000, "Intel Iris Pro Graphics 5200" },
	{ 0x0A26, 0x0000, "Intel HD Graphics 5000" },
	{ 0x0A2A, 0x0A2E, "Intel Iris Graphics 5100" },
	{ 0x0A2B, 0x0A2E, "Intel Iris Graphics 5100" },
	{ 0x0A2E, 0x0000, "Intel Iris Graphics 5100" },
	{ 0x0412, 0x0000, "Intel HD Graphics 4600" },
	{ 0x0416, 0x0412, "Intel HD Graphics 4600" },
	{ 0x041A, 0x0412, "Intel HD Graphics P4600" },
	{ 0x041B, 0x0412, nullptr },
	{ 0x041E, 0x0412, "Intel HD Graphics 4400" },
	{ 0x0A12, 0x0412, nullptr },
	{ 0x0A16, 0x0412, "Intel HD Graphics 4400" },
	{ 0x0A1A, 0x0412, nullptr },
	{ 0x0A1E, 0x0412, "Intel HD Graphics 4200" },
	{ 0x0A22, 0x0A2E, "Intel Iris Graphics 5100" },
	{ 0x0D12, 0x0412, "Intel HD Graphics 4600" },
	{ 0x0D16, 0x0412, "Intel HD Graphics 4600" },
	{ 0x1612, 0x0000, "Intel HD Graphics 5600" },
	{ 0x1616, 0x0000, "Intel HD Graphics 5500" },
	{ 0x161E, 0x0000, "Intel HD Graphics 5300" },
	{ 0x1622, 0x0000, "Intel Iris Pro Graphics 6200" },
	{ 0x1626, 0x0000, "Intel HD Graphics 6000" },
	{ 0x162B, 0x0000, "Intel Iris Graphics 6100" },
	{ 0x162A, 0x0000, "Intel Iris Pro Graphics P6300" },
	{ 0x162D, 0x0000, "Intel Iris Pro Graphics P6300" },
	// Reserved/unused/generic Broadwell },
	// { 0x0BD1, 0x0000, nullptr },
	// { 0x0BD2, 0x0000, nullptr },
	// { 0x0BD3, 0x0000, nullptr },
	// { 0x1602, 0x0000, nullptr },
	// { 0x1606, 0x0000, nullptr },
	// { 0x160B, 0x0000, nullptr },
	// { 0x160A, 0x0000, nullptr },
	// { 0x160D, 0x0000, nullptr },
	// { 0x160E, 0x0000, nullptr },
	// { 0x161B, 0x0000, nullptr },
	// { 0x161A, 0x0000, nullptr },
	// { 0x161D, 0x0000, nullptr },
	// { 0x162E, 0x0000, nullptr },
	// { 0x1632, 0x0000, nullptr },
	// { 0x1636, 0x0000, nullptr },
	// { 0x163B, 0x0000, nullptr },
	// { 0x163A, 0x0000, nullptr },
	// { 0x163D, 0x0000, nullptr },
	// { 0x163E, 0x0000, nullptr },
	{ 0x1902, 0x0000, "Intel HD Graphics 510" },
	{ 0x1906, 0x0000, "Intel HD Graphics 510" },
	{ 0x190B, 0x0000, "Intel HD Graphics 510" },
	{ 0x191E, 0x0000, "Intel HD Graphics 515" },
	{ 0x1916, 0x0000, "Intel HD Graphics 520" },
	{ 0x1921, 0x0000, "Intel HD Graphics 520" },
	{ 0x1912, 0x0000, "Intel HD Graphics 530" },
	{ 0x191B, 0x0000, "Intel HD Graphics 530" },
	{ 0x191D, 0x191B, "Intel HD Graphics P530" },
	{ 0x1923, 0x191B, "Intel HD Graphics 535" },
	{ 0x1926, 0x0000, "Intel Iris Graphics 540" },
	{ 0x1927, 0x0000, "Intel Iris Graphics 550" },
	{ 0x192B, 0x0000, "Intel Iris Graphics 555" },
	{ 0x192D, 0x1927, "Intel Iris Graphics P555" },
	{ 0x1932, 0x0000, "Intel Iris Pro Graphics 580" },
	{ 0x193A, 0x193B, "Intel Iris Pro Graphics P580" },
	{ 0x193B, 0x0000, "Intel Iris Pro Graphics 580" },
	{ 0x193D, 0x193B, "Intel Iris Pro Graphics P580" },
	// Reserved/unused/generic Skylake },
	// { 0x0901, 0x0000, nullptr },
	// { 0x0902, 0x0000, nullptr },
	// { 0x0903, 0x0000, nullptr },
	// { 0x0904, 0x0000, nullptr },
	// { 0x190E, 0x0000, nullptr },
	// { 0x1913, 0x0000, nullptr },
	// { 0x1915, 0x0000, nullptr },
	// { 0x1917, 0x0000, nullptr },
	{ 0x5902, 0x591E, "Intel HD Graphics 610" },
	{ 0x591E, 0x0000, "Intel HD Graphics 615" },
	{ 0x5916, 0x0000, "Intel HD Graphics 620" },
	{ 0x5917, 0x5916, "Intel UHD Graphics 620" },
	{ 0x5912, 0x0000, "Intel HD Graphics 630" },
	{ 0x591B, 0x0000, "Intel HD Graphics 630" },
	{ 0x591C, 0x0000, "Intel UHD Graphics 615" },
	{ 0x591D, 0x591B, "Intel HD Graphics P630" },
	{ 0x5923, 0x0000, "Intel HD Graphics 635" },
	{ 0x5926, 0x0000, "Intel Iris Plus Graphics 640" },
	{ 0x5927, 0x0000, "Intel Iris Plus Graphics 650" },
	{ 0x87C0, 0x0000, "Intel UHD Graphics 617" },
	// Reserved/unused/generic Kaby Lake / Amber Lake},
	{ 0x3E90, 0x3E92, "Intel UHD Graphics 610" },
	{ 0x3E91, 0x0000, "Intel UHD Graphics 630" },
	{ 0x3E92, 0x0000, "Intel UHD Graphics 630" },
	{ 0x3E93, 0x3E92, "Intel UHD Graphics 610" },
	{ 0x3E94, 0x3E92, "Intel UHD Graphics P630" },
	{ 0x3E96, 0x3E92, "Intel UHD Graphics P630" },
	{ 0x3E98, 0x0000, "Intel UHD Graphics 630" },
	{ 0x3E9A, 0x3E92, "Intel UHD Graphics P630" },
	{ 0x3E9B, 0x0000, "Intel UHD Graphics 630" },
	{ 0x3EA0, 0x3EA5, "Intel UHD Graphics 620" },
	{ 0x3EA5, 0x0000, "Intel Iris Plus Graphics 655" },
	{ 0x3EA6, 0x0000, "Intel Iris Plus Graphics 645" },
	{ 0x9BC4, 0x0000, "Intel UHD Graphics 630" },
	{ 0x9BC5, 0x0000, "Intel UHD Graphics 630" },
	{ 0x9BC6, 0x9BC5, "Intel UHD Graphics P630" },
	{ 0x9BC8, 0x0000, "Intel UHD Graphics 630" },
	{ 0x9BE6, 0x9BC5, "Intel UHD Graphics P630" },
	// Reserved/unused/generic Coffee Lake / Whiskey Lake / Comet Lake},
	{ 0x8A51, 0x0000, "Intel Iris Plus Graphics" },
	{ 0x8A52, 0x0000, "Intel Iris Plus Graphics" },
	{ 0x8A53, 0x0000, "Intel Iris Plus Graphics" },
	{ 0x8A5A, 0x0000, "Intel Iris Plus Graphics" },
	{ 0x8A5C, 0x0000, "Intel Iris Plus Graphics" },
	// Reserved/unused/generic Ice Lake },
	// { 0x9A49, 0x0000, "Intel Iris Xe Graphics" },
	// above line is undefined because its unsure and causes loops
	{ 0x9A49, 0x8A52, "Intel Iris Xe Graphics" },
	{ 0x9A40, 0x8A52, "Intel Iris Xe Graphics" },
	{ 0x9A60, 0x8A5A, "Intel Iris Xe Graphics" },
	{ 0x9A68, 0x8A5A, "Intel Iris Xe Graphics" },
	{ 0x9A70, 0x8A5A, "Intel Iris Xe Graphics" },
	{ 0x9A78, 0x8A5A, "Intel Iris Xe Graphics" },
	// Reserved/unused/generic Tiger Lake },
	// The above lines points to irisxe graphics, with address
};

const char *WEG::getIntelModel(uint32_t dev, uint32_t &fakeId) {
	fakeId = 0;
	for (size_t i = 0; i < arrsize(devIntel); i++) {
		if (devIntel[i].device == dev) {
			fakeId = devIntel[i].fake;
			return devIntel[i].name;
		}
	}

	return nullptr;
}
