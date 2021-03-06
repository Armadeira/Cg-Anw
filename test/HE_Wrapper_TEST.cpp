//
// Created by ZielM on 30.04.2018.
//

#include <SimpleObjParser.h>
#include "HalfEdge.h"
#include "gtest/gtest.h"

namespace cg {

// The fixture for testing class Foo.
    class HE_WrapperTest : public ::testing::Test {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        HE_WrapperTest() {
            // You can do set-up work for each test here.
        }

        virtual ~HE_WrapperTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() {
            // Code here will be called immediately after the constructor (right
            // before each test).

            setupCube();
        }

        void setupCube() {
            std::vector<glm::vec3> vertsExample = {
                    {glm::vec3(-1.0, -1.0, 1.0)},
                    {glm::vec3(1.0, -1.0, 1.0)},
                    {glm::vec3(-1.0, 1.0, 1.0)},
                    {glm::vec3(1.0, 1.0, 1.0)},
                    {glm::vec3(-1.0, 1.0, -1.0)},
                    {glm::vec3(1.0, 1.0, -1.0)},
                    {glm::vec3(-1.0, -1.0, -1.0)},
                    {glm::vec3(1.0, -1.0, -1.0)}
            };

            std::vector<std::vector<int>> faces = {
                    {0, 1, 2},
                    {2, 1, 3},

                    {2, 3, 4},
                    {4, 3, 5},

                    {4, 5, 6},
                    {6, 5, 7},

                    {6, 7, 0},
                    {0, 7, 1},

                    {1, 7, 3},
                    {3, 7, 5},

                    {6, 0, 4},
                    {4, 0, 2}
            };

            this->he_wrapper = SimpleObjParser().create(vertsExample, faces);


        }

        virtual void TearDown() {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }

        // Objects declared here can be used by all tests in the test case for Foo.
        std::shared_ptr<HE_Wrapper> he_wrapper;
    };



    TEST_F(HE_WrapperTest, DeleteFace) {
        ASSERT_TRUE(he_wrapper->getFaces().size() > 0);

        FacePointer firstFace = he_wrapper->getFaces()[0];
        he_wrapper->deleteFace(firstFace);
    }

}  // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}