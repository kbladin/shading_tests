#include <iostream>
#include "gtest/gtest.h"
#include <ElasticMaterials_lib/MCS.h>

class EMTest : public ::testing::Test {
protected:
	EMTest() {

	}

	virtual ~EMTest() {

	}

	virtual void SetUp() {

	}

	virtual void TearDown() {

	}
	
};

TEST_F(EMTest, superMegaTestet) {
	int right = 0;
	std::cout << "Running test function and shit." << std::endl;
	EXPECT_EQ(0,right);
}

TEST_F(EMTest, ParticleConnectionMCS) {

}

TEST_F(EMTest, UVtest) {

}