
CFLAGS := -std=c++11 -Wall -Wextra -pedantic -ggdb 
# CFLAGS := $(CFLAGS) -O2 -fno-omit-frame-pointer -pg
LDFLAGS := -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-system -lstdc++ -lm

test_level: collision_layers.o Killable.o Animation.o Bomb.o AnimatedSprite.o Animated.o EntityType.o GameCache.o Level.o test_level.o LevelSet.o LevelNumText.o Music.o Game.o Texture.o Points.o ShadedText.o Track.o utils.o Entity.o EntityGroup.o Coin.o AxisMoving.o Moving.o controls.o Shooting.o MovingAnimator.o Player.o Enemy.o AlienSprite.o Direction.o Teleport.o Flash.o BossExplosion.o FixedWall.o BreakableWall.o TransparentWall.o Sprite.o Bullet.o AxisBullet.o FreeBullet.o FreeMoving.o Explosion.o Screen.o ScreenHandler.o HomeScreen.o Controllable.o ControlsScreen.o PreferencesScreen.o AboutScreen.o PauseScreen.o Sighted.o CollisionDetector.o Collider.o LevelManager.o LevelLoader.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
points: Killable.o Animation.o AnimatedSprite.o Animated.o EntityType.o GameCache.o Level.o LevelSet.o LevelNumText.o Game.o Texture.o Points.o ShadedText.o utils.o Entity.o EntityGroup.o AxisMoving.o Moving.o controls.o MovingAnimator.o Direction.o Sprite.o FreeMoving.o Track.o Music.o test_points.o Explosion.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf *.o test_level

test_level.o: tests/test_level.cpp
	$(CC) $(CFLAGS) -c $<
test_points.o: tests/test_points.cpp
	$(CC) $(CFLAGS) -c $<
Killable.o: src/Killable.cpp
	$(CC) $(CFLAGS) -c $<
Animation.o: src/Animation.cpp
	$(CC) $(CFLAGS) -c $<
Bomb.o: src/Bomb.cpp
	$(CC) $(CFLAGS) -c $<
AnimatedSprite.o: src/AnimatedSprite.cpp
	$(CC) $(CFLAGS) -c $<
Animated.o: src/Animated.cpp
	$(CC) $(CFLAGS) -c $<
EntityType.o: src/EntityType.cpp
	$(CC) $(CFLAGS) -c $<
GameCache.o: src/GameCache.cpp
	$(CC) $(CFLAGS) -c $<
Level.o: src/Level.cpp
	$(CC) $(CFLAGS) -c $<
LevelSet.o: src/LevelSet.cpp
	$(CC) $(CFLAGS) -c $<
LevelNumText.o: src/LevelNumText.cpp
	$(CC) $(CFLAGS) -c $<
Music.o: src/Music.cpp
	$(CC) $(CFLAGS) -c $<
Game.o: src/Game.cpp
	$(CC) $(CFLAGS) -c $<
Texture.o: src/Texture.cpp
	$(CC) $(CFLAGS) -c $<
ShadedText.o: src/ShadedText.cpp
	$(CC) $(CFLAGS) -c $<
Track.o: src/Track.cpp
	$(CC) $(CFLAGS) -c $<
utils.o: src/utils.cpp
	$(CC) $(CFLAGS) -c $<
Entity.o: src/Entity.cpp
	$(CC) $(CFLAGS) -c $<
EntityGroup.o: src/EntityGroup.cpp 
	$(CC) $(CFLAGS) -c $<
Coin.o: src/Coin.cpp
	$(CC) $(CFLAGS) -c $<
AxisMoving.o: src/AxisMoving.cpp
	$(CC) $(CFLAGS) -c $<
Moving.o: src/Moving.cpp
	$(CC) $(CFLAGS) -c $<
Player.o: src/Player.cpp
	$(CC) $(CFLAGS) -c $<
controls.o: src/controls.cpp
	$(CC) $(CFLAGS) -c $<
Shooting.o: src/Shooting.cpp
	$(CC) $(CFLAGS) -c $<
MovingAnimator.o: src/MovingAnimator.cpp
	$(CC) $(CFLAGS) -c $<
Enemy.o: src/Enemy.cpp
	$(CC) $(CFLAGS) -c $<
AlienSprite.o: src/AlienSprite.cpp
	$(CC) $(CFLAGS) -c $<
Direction.o: src/Direction.cpp
	$(CC) $(CFLAGS) -c $<
Teleport.o: src/Teleport.cpp
	$(CC) $(CFLAGS) -c $<
Flash.o: src/Flash.cpp
	$(CC) $(CFLAGS) -c $<
FixedWall.o: src/FixedWall.cpp
	$(CC) $(CFLAGS) -c $<
BreakableWall.o: src/BreakableWall.cpp
	$(CC) $(CFLAGS) -c $<
TransparentWall.o: src/TransparentWall.cpp
	$(CC) $(CFLAGS) -c $<
BossExplosion.o: src/BossExplosion.cpp
	$(CC) $(CFLAGS) -c $<
Sprite.o: src/Sprite.cpp
	$(CC) $(CFLAGS) -c $<
Bullet.o: src/Bullet.cpp
	$(CC) $(CFLAGS) -c $<
AxisBullet.o: src/AxisBullet.cpp
	$(CC) $(CFLAGS) -c $<
FreeBullet.o: src/FreeBullet.cpp
	$(CC) $(CFLAGS) -c $<
FreeMoving.o: src/FreeMoving.cpp
	$(CC) $(CFLAGS) -c $<
Explosion.o: src/Explosion.cpp
	$(CC) $(CFLAGS) -c $<
Screen.o: src/Screen.cpp
	$(CC) $(CFLAGS) -c $<
HomeScreen.o: src/HomeScreen.cpp
	$(CC) $(CFLAGS) -c $<
PreferencesScreen.o: src/PreferencesScreen.cpp
	$(CC) $(CFLAGS) -c $<
AboutScreen.o: src/AboutScreen.cpp
	$(CC) $(CFLAGS) -c $<
ControlsScreen.o: src/ControlsScreen.cpp
	$(CC) $(CFLAGS) -c $<
PauseScreen.o: src/PauseScreen.cpp
	$(CC) $(CFLAGS) -c $<
ScreenHandler.o: src/ScreenHandler.cpp
	$(CC) $(CFLAGS) -c $<
Points.o: src/Points.cpp
	$(CC) $(CFLAGS) -c $<
Controllable.o: src/Controllable.cpp
	$(CC) $(CFLAGS) -c $<
SaveManager.o: src/SaveManager.cpp
	$(CC) $(CFLAGS) -c $<
Sighted.o: src/Sighted.cpp
	$(CC) $(CFLAGS) -c $<
CollisionDetector.o: src/CollisionDetector.cpp
	$(CC) $(CFLAGS) -c $<
Collider.o: src/Collider.cpp
	$(CC) $(CFLAGS) -c $<
collision_layers.o: src/collision_layers.cpp
	$(CC) $(CFLAGS) -c $<
LevelManager.o: src/LevelManager.cpp
	$(CC) $(CFLAGS) -c $<
LevelLoader.o: src/LevelLoader.cpp
	$(CC) $(CFLAGS) -c $<
