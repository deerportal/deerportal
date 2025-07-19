#include "boarddiamondseq.h"

BoardDiamondSeq::BoardDiamondSeq(TextureHolder* textures)
    : m_vertices(sf::PrimitiveType::Triangles, DP::diamondsNumber * 6), m_needsUpdate(true) {
  this->textures = textures;
  for (int i = 0; i < DP::diamondsNumber; i++) {
    diamonds[i] = BoardDiamond(this->textures, DP::DIAMONDS_SETUP[i][0], DP::DIAMONDS_SETUP[i][1],
                               DP::DIAMONDS_SETUP[i][2]);
  }
  reorder();
}

void BoardDiamondSeq::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // PERFORMANCE OPTIMIZATION (Issue #68):
  // Use VertexArray for batched rendering instead of 112 individual draw calls

  if (m_needsUpdate) {
    updateVertexArray();
    m_needsUpdate = false;
  }

  // DEBUG: Try without transform to fix positioning
  // states.transform *= getTransform();
  states.texture = &textures->textureBoardDiamond;

  // Single draw call for all 112 diamonds!
  target.draw(m_vertices, states);
}

void BoardDiamondSeq::updateVertexArray() const {
  // Update the entire vertex array when diamonds change position
  for (int i = 0; i < DP::diamondsNumber; i++) {
    updateSingleDiamond(i);
  }
}

void BoardDiamondSeq::updateSingleDiamond(int index) const {
  // Calculate vertex array offset for this diamond (6 vertices per 2 triangles)
  int vertexIndex = index * 6;

  const BoardDiamond& diamond = diamonds[index];

  // If diamond is not on board (position -1), make it invisible
  if (diamond.boardPosition == -1) {
    // Set all vertices to same position to make diamond invisible
    for (int v = 0; v < 6; v++) {
      m_vertices[vertexIndex + v].position = sf::Vector2f(0, 0);
      m_vertices[vertexIndex + v].texCoords = sf::Vector2f(0, 0);
      m_vertices[vertexIndex + v].color = sf::Color::Transparent;
    }
    return;
  }

  // NEW APPROACH: Calculate position to align perfectly with board tiles
  // Use the board position directly and align with tile grid
  sf::Vector2i cords = DP::transPosition(diamond.boardPosition);
  sf::Vector2f tilePos = DP::getScreenPos(cords);

  // Pre-calculated constants for optimal performance (no runtime calculations)
  const float diamondSize = 35.2f; // 44.0f * 0.8f = 35.2f (80% scaled)
  const float offsetX = 2.4f; // (40 - 35.2) / 2 = 2.4f (centering offset)
  const float offsetY = 2.4f; // (40 - 35.2) / 2 = 2.4f (centering offset)

  sf::Vector2f position(tilePos.x + offsetX, tilePos.y + offsetY);

  // Calculate texture coordinates based on diamond ID (use full texture size)
  int idNumber = diamond.idNumber;
  const float textureSize = 44.0f; // Original texture size
  float texLeft = idNumber * textureSize;
  float texTop = 0.0f;
  float texRight = texLeft + textureSize;
  float texBottom = texTop + textureSize;

  // SFML 3: Use triangles instead of quads (2 triangles = 6 vertices)
  // Triangle 1: top-left, top-right, bottom-left
  m_vertices[vertexIndex + 0].position = sf::Vector2f(position.x, position.y);
  m_vertices[vertexIndex + 1].position = sf::Vector2f(position.x + diamondSize, position.y);
  m_vertices[vertexIndex + 2].position = sf::Vector2f(position.x, position.y + diamondSize);

  // Triangle 2: top-right, bottom-right, bottom-left
  m_vertices[vertexIndex + 3].position = sf::Vector2f(position.x + diamondSize, position.y);
  m_vertices[vertexIndex + 4].position =
      sf::Vector2f(position.x + diamondSize, position.y + diamondSize);
  m_vertices[vertexIndex + 5].position = sf::Vector2f(position.x, position.y + diamondSize);

  // Triangle 1 texture coordinates
  m_vertices[vertexIndex + 0].texCoords = sf::Vector2f(texLeft, texTop);
  m_vertices[vertexIndex + 1].texCoords = sf::Vector2f(texRight, texTop);
  m_vertices[vertexIndex + 2].texCoords = sf::Vector2f(texLeft, texBottom);

  // Triangle 2 texture coordinates
  m_vertices[vertexIndex + 3].texCoords = sf::Vector2f(texRight, texTop);
  m_vertices[vertexIndex + 4].texCoords = sf::Vector2f(texRight, texBottom);
  m_vertices[vertexIndex + 5].texCoords = sf::Vector2f(texLeft, texBottom);

  // Set color (white for normal rendering)
  sf::Color diamondColor = sf::Color::White;
  for (int v = 0; v < 6; v++) {
    m_vertices[vertexIndex + v].color = diamondColor;
  }
}

std::array<int, DP::diamondsNumber> BoardDiamondSeq::getBoardPositions() {
  std::array<int, DP::diamondsNumber> results;
  for (int i = 0; i < DP::diamondsNumber; i++) {
    results[i] = diamonds[i].getBoardPosition();
  }
  return results;
}

// bool BoardDiamondSeq::ifFieldIsEmpty(int pos, int element)
//{
//     for (int
//     i=element*DP::diamondsNumber/4;i<(DP::diamondsNumber/4)+(element*DP::diamondsNumber/4);i++)
//     {
//         if (diamonds[i].boardPosition==pos)
//             return false;
//     }
//     return true;

//}

bool BoardDiamondSeq::ifFieldIsEmpty(int pos) {
  for (int element = 0; element < 4; element++) {
    for (int i = element * DP::diamondsNumber / 4;
         i < (DP::diamondsNumber / 4) + (element * DP::diamondsNumber / 4); i++) {
      if (diamonds[i].boardPosition == pos) return false;
    }
  }
  return true;
}

int BoardDiamondSeq::getRandomPos(int playerNumber) {
  std::set<int> setSteps(DP::occupiedFields[playerNumber].cbegin(),
                         DP::occupiedFields[playerNumber].cend());
  for (int i = playerNumber * DP::diamondsNumber / 4;
       i < (DP::diamondsNumber / 4) + (playerNumber * DP::diamondsNumber / 4); i++) {
    if (diamonds[i].boardPosition != -1) {
      setSteps.erase(diamonds[i].boardPosition);
    }
  }
  int step = rand() % setSteps.size();
  std::vector<int> freePositions;
  freePositions.resize(setSteps.size());
  freePositions.assign(setSteps.begin(), setSteps.end());
  return freePositions[step];
}

void BoardDiamondSeq::reorder() {
  srand((unsigned)time(0));

  for (int element = 0; element < 4; element++) {
    int start = element;
    for (int i = start * DP::diamondsNumber / 4;
         i < (DP::diamondsNumber / 4) + (start * DP::diamondsNumber / 4); i++) {
      int step = getRandomPos(element);
      diamonds[i].setBoardPosition(step);
    }
  }

  // Mark vertex array for update after reordering
  m_needsUpdate = true;
}

void BoardDiamondSeq::reorder(int element) {
  srand((unsigned)time(0));
  int start = element;
  if (element == 2)
    start = 3;
  else if (element == 3)
    start = 2;

  int newElement = start;

  for (int i = start * DP::diamondsNumber / 4;
       i < (DP::diamondsNumber / 4) + (start * DP::diamondsNumber / 4); i++) {
    int step = getRandomPos(newElement);
    diamonds[i].setBoardPosition(step);
  }

  // Mark vertex array for update after reordering
  m_needsUpdate = true;
}

void BoardDiamondSeq::collectField(int pos) {
  for (int i = 0; i < DP::diamondsNumber; i++) {
    if (diamonds[i].boardPosition == pos) {
      diamonds[i].setBoardPosition(-1);

      // Mark vertex array for update after collecting diamond
      m_needsUpdate = true;
      break;
    }
  }
}

int BoardDiamondSeq::getNumberForField(int pos) {
  for (int i = 0; i < DP::diamondsNumber; i++) {
    if (diamonds[i].boardPosition == pos) {
      return diamonds[i].idNumber;
    }
  }
  return -1;
}

