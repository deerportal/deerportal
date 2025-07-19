#ifndef BOARDDIAMONDSEQ_H
#define BOARDDIAMONDSEQ_H
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>

#include <SFML/Graphics.hpp>

#include "boarddiamond.h"
#include "textureholder.h"

// Forward declaration to avoid circular dependency
namespace DP {
class WindowManager;
}

/*!
 * \brief The BoardDiamondSeq class
 * We are going to have 2 cards / diamonds of each element per each area,
 * which gives 2*4*4 = 32 cards diamonds of elements on the board.
 *
 * Additionally there will be extra 6 white diamonds per area to collect, which
 * gives 4*6 = 24 diamonds on the board.
 *
 * Together it would give 32 + 24 = 56 diamonds cards/diamonds together,
 * 15 per area.
 *
 * PERFORMANCE OPTIMIZATION (Issue #68):
 * Now uses VertexArray for efficient batched rendering instead of 112 individual draw calls.
 */

class BoardDiamondSeq : public sf::Drawable, public sf::Transformable {
public:
  explicit BoardDiamondSeq(TextureHolder* textures, DP::WindowManager* windowManager = nullptr);
  TextureHolder* textures;
  DP::WindowManager* windowManager;
  BoardDiamond diamonds[DP::diamondsNumber];

  std::array<int, DP::diamondsNumber> getBoardPositions();
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  int getRandomPos(int playerNumber);

  void reorder();

  //    bool ifFieldIsEmpty(int pos, int element);
  bool ifFieldIsEmpty(int pos);

  void collectField(int pos);
  int getNumberForField(int pos);

  void reorder(int element);
  
  void markForUpdate(); // Force vertex array update (e.g., after fullscreen toggle)

private:
  // PERFORMANCE OPTIMIZATION: VertexArray for batched rendering (Issue #68)
  mutable sf::VertexArray m_vertices;
  mutable bool m_needsUpdate;

  // Helper methods for VertexArray optimization
  void updateVertexArray() const;
  void updateSingleDiamond(int index) const;
};

#endif // BOARDDIAMONDSEQ_H
