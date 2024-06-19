#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../src/Game/Game.h"

TEST_CASE("Case 1")
{
	static constexpr unsigned int SCALE = 4;
	static constexpr  unsigned int BLOCK_SIZE = 16;
	glm::uvec2 g_windowSize(SCALE * 10 * BLOCK_SIZE, SCALE * 12 * BLOCK_SIZE); 
	std::unique_ptr<Game> g_game = std::make_unique<Game>(g_windowSize);
	CHECK(SCALE == 4);
}