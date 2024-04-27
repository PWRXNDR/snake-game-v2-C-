#include <SFML/Graphics.hpp>
#include <list>
#include <cstdlib>
#include <ctime>

const int WIDTH = 800;
const int HEIGHT = 600;
const int BLOCK_SIZE = 20;

struct Block {
    int x, y;
    Block(int xCoord, int yCoord) : x(xCoord), y(yCoord) {}
};

std::list<Block> snake;
Block food(300, 300);

void initGame() {
    snake.clear();
    snake.push_back(Block(WIDTH / 2, HEIGHT / 2));
    snake.push_back(Block(WIDTH / 2 + BLOCK_SIZE, HEIGHT / 2));
    snake.push_back(Block(WIDTH / 2 + 2 * BLOCK_SIZE, HEIGHT / 2));

    // Initialize random seed
    std::srand(std::time(0));

    // Random position for the first food
    food.x = (std::rand() % (WIDTH / BLOCK_SIZE)) * BLOCK_SIZE;
    food.y = (std::rand() % (HEIGHT / BLOCK_SIZE)) * BLOCK_SIZE;
}

void draw(sf::RenderWindow& window) {
    window.clear();

    // Draw the snake
    for (auto& block : snake) {
        sf::RectangleShape rectangle(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));
        rectangle.setFillColor(sf::Color::Green);
        rectangle.setPosition(block.x, block.y);
        window.draw(rectangle);
    }

    // Draw the food
    sf::RectangleShape foodShape(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));
    foodShape.setFillColor(sf::Color::Red);
    foodShape.setPosition(food.x, food.y);
    window.draw(foodShape);

    window.display();
}

bool checkCollision() {
    // Check if the snake hits the boundaries
    if (snake.front().x < 0 || snake.front().x >= WIDTH ||
        snake.front().y < 0 || snake.front().y >= HEIGHT) {
        return true;
    }

    // Check if the snake hits itself
    for (auto it = ++snake.begin(); it != snake.end(); ++it) {
        if (it->x == snake.front().x && it->y == snake.front().y) {
            return true;
        }
    }
    return false;
}

void moveSnake(int dx, int dy) {
    Block newHead(snake.front().x + dx * BLOCK_SIZE, snake.front().y + dy * BLOCK_SIZE);
    snake.push_front(newHead);

    if (newHead.x == food.x && newHead.y == food.y) {
        // Random position for new food
        food.x = (std::rand() % (WIDTH / BLOCK_SIZE)) * BLOCK_SIZE;
        food.y = (std::rand() % (HEIGHT / BLOCK_SIZE)) * BLOCK_SIZE;
    } else {
        snake.pop_back();
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML Snake Game");
    initGame();

    int dx = 1, dy = 0;
    sf::Clock clock;
    float timer = 0, delay = 0.1;

    while (window.isOpen()) {
        float time = clock.restart().asSeconds();
        timer += time;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && dy == 0) {
            dx = 0; dy = -1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && dy == 0) {
            dx = 0; dy = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && dx == 0) {
            dx = -1; dy = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && dx == 0) {
            dx = 1; dy = 0;
        }

        if (timer > delay) {
            timer = 0;
            moveSnake(dx, dy);
            if (checkCollision()) {
                initGame(); // Restart the game on collision
            }
        }

        draw(window);
    }

    return 0;
}
