#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>				
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <math.h>
#include <time.h>
#include "Header.h"

//GLOBALS
const int width = 1366;
const int height = 768;
const int NUM_BULLETS = 5;
const int NUM_ENEMYS = 10;
const int maxFrame = 8;

enum KEYS{LEFT,RIGHT};
bool keys[2] = { FALSE, FALSE };


//PROTOTYPES
void InitPlayer(Player &player);
void DrawPlayer(Player &player,ALLEGRO_BITMAP *czarodziej,ALLEGRO_BITMAP *czarodziej2,bool postac);
void MovePlayerLeft(Player &player);
void MovePlayerRight(Player &player);

void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, Player &player);
void UpdateBullet(Bullet bullet[], int size,double  pozycja_myszki_X, double  pozycja_myszki_Y);

void InitEnemy(Enemy &enemy);
void DrawEnemy(Enemy &enemy, int maxFrame, int curFrame, int frameCount, int frameDelay, int frameWidth, int frameHeight, ALLEGRO_BITMAP *wrogowie);
void StartEnemy(Enemy &enemy);
void UpdateEnemy(Enemy &enemy, int maxFrame, int curFrame, int frameCount, int frameDelay, int frameWidth, int frameHeight, ALLEGRO_BITMAP *wrogowie, ALLEGRO_BITMAP *czarodziej2);

void InitTower(Tower &tower);
void DrawTower(Tower &tower,ALLEGRO_BITMAP *wieza);
void UpdateTower(Tower &tower);

int main(void)
{
	bool done = false;
	bool postac = false; // ZMIENNA DO RUCHU POSTACI
	bool redraw = true;
	const int FPS = 60;
	double pozycja_myszki_X;
	double pozycja_myszki_Y;

	int curFrame = 0;
	int frameCount = 0;
	int frameDelay = 5;
	int frameWidth = 122;
	int frameHeight = 128;

	//OBJECTS VARIABLES
	Player player;
	Tower tower;
	Bullet bullets[NUM_BULLETS];
	Enemy enemy;

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	//BITMAPS
	ALLEGRO_BITMAP *czarodziej = NULL;
	ALLEGRO_BITMAP *czarodziej2 = NULL;
	ALLEGRO_BITMAP *mapa = NULL;
	ALLEGRO_BITMAP *wieza = NULL;
	ALLEGRO_BITMAP *czarodziej_strzal1 = NULL;
	ALLEGRO_BITMAP *czarodziej_strzal2 = NULL;
	ALLEGRO_BITMAP *wrogowie = NULL;
	ALLEGRO_BITMAP *ataki = NULL;
	//TIMER
	ALLEGRO_TIMER *timer = NULL;


	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL,
			"FAILURE TO INITIATE ALLEGRO!", NULL, NULL);
		return -1;
	}
	
	//ALLEGRO VARIABLES
	display = al_create_display(width, height);
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	if (!display)
	{
		al_show_native_message_box(NULL, NULL, NULL,
			"FAILURE TO INITIATE ALLEGRO!", NULL, NULL);
		return -1;
	}

	//INITS

	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_mouse();

	// INIT FUNCTIONS.
	srand(time(NULL));
	InitPlayer(player);
	InitBullet(bullets, NUM_BULLETS);
	InitEnemy(enemy);
	InitTower(tower);

	// BITMAPS
	mapa = al_load_bitmap("mapa.png");
	czarodziej = al_load_bitmap("2.png");
	czarodziej2 = al_load_bitmap("2a.png");
	czarodziej_strzal1 = al_load_bitmap("strzelajacylewo.png");
	czarodziej_strzal2 = al_load_bitmap("strzelajacyprawo.png");
	wrogowie = al_load_bitmap("wrogie.png");
	wieza = al_load_bitmap("wiezyczka2.png");
	ataki = al_load_bitmap("ataki.png");

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;

			if (keys[LEFT])
				MovePlayerLeft(player);

			if (keys[RIGHT])
				MovePlayerRight(player);
			
			UpdateBullet(bullets, NUM_BULLETS,pozycja_myszki_X,pozycja_myszki_Y);
			UpdateEnemy(enemy, maxFrame, curFrame, frameCount, frameDelay, frameWidth, frameHeight, wrogowie,czarodziej2);
			StartEnemy(enemy);
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
					switch (ev.keyboard.keycode)
					{
					case ALLEGRO_KEY_ESCAPE:
						done = true;
						break;
					case ALLEGRO_KEY_LEFT:
						keys[RIGHT] = true;
						postac = true;
						break;
					case ALLEGRO_KEY_RIGHT:
						keys[LEFT] = true;
						postac = false;
						break;

					}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
				switch (ev.keyboard.keycode)
					{
						case ALLEGRO_KEY_ESCAPE:
							done = true;
							break;
						case ALLEGRO_KEY_LEFT:
							keys[RIGHT] = false;
							break;
						case ALLEGRO_KEY_RIGHT:
							keys[LEFT] = false;
							break;
					}
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			pozycja_myszki_X = ev.mouse.x;
			pozycja_myszki_Y = ev.mouse.y;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (ev.mouse.button & 1)
			{
				if (!postac)
				FireBullet(bullets, NUM_BULLETS, player);
				al_flip_display();
			}
		}
	
		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;
			al_draw_bitmap(mapa, 0, 0, 0);
			DrawBullet(bullets, NUM_BULLETS);
			DrawTower(tower, wieza);
			DrawEnemy(enemy, maxFrame, curFrame, frameCount, frameDelay, frameWidth, frameHeight, wrogowie);
			DrawPlayer(player, czarodziej, czarodziej2, postac);
			al_flip_display();
		}
		
	}



	al_destroy_bitmap(wrogowie);
	al_destroy_bitmap(mapa);
	al_destroy_bitmap(czarodziej_strzal1);
	al_destroy_bitmap(czarodziej_strzal2);
	al_destroy_bitmap(czarodziej);
	al_destroy_bitmap(czarodziej2);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);

	return 0;
}
void InitPlayer(Player &player)
{
	player.x = 115;
	player.y = 140;
	player.ID = PLAYER;
	player.speed = 3;
	player.score = 0;
}
void DrawPlayer(Player &player, ALLEGRO_BITMAP *czarodziej, ALLEGRO_BITMAP *czarodziej2,bool postac)
{
	if (!postac)
	al_draw_bitmap(czarodziej, player.x, player.y, 0);// 90-120
	else
	al_draw_bitmap(czarodziej2, player.x, player.y, 0);// 90-120

}
void MovePlayerLeft(Player &player)
{
	player.x += player.speed;
	if (player.x > 120)
		player.x = 120;

}
void MovePlayerRight(Player &player)
{
	player.x -= player.speed;
	if (player.x < 90)
		player.x = 90;
}

void InitBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		bullet[i].ID = BULLET;
		bullet[i].speedx = 0;
		bullet[i].speedy = 0;
		bullet[i].live = false;
		bullet[i].angle = 0;
	}
}
void DrawBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (bullet[i].live)
			al_draw_filled_circle(bullet[i].x, bullet[i].y + 130,2, al_map_rgb(255, 0, 0));
	}
}
void FireBullet(Bullet bullet[], int size,Player &player)
{
	for (int i = 0; i < size; i++)
	{
		if (!bullet[i].live)
		{
			bullet[i].x = player.x + 145;
			bullet[i].y = player.y;
			bullet[i].live = true;
			break;
		}
	}
}
void UpdateBullet(Bullet bullet[], int size, double  pozycja_myszki_X, double  pozycja_myszki_Y)
{
	for (int i = 0; i < size; i++)
	{
		if (bullet[i].live)
		{
			bullet[i].angle =atan2(pozycja_myszki_X,pozycja_myszki_Y);
			bullet[i].x += sin(bullet[i].angle) * 5;
			bullet[i].y += cos(bullet[i].angle) * 5;
			if (bullet[i].x > width || bullet[i].y>height)
				bullet[i].live = false;
		}

	}
}

void InitEnemy(Enemy &enemy)
{
		enemy.ID = ENEMY;
		enemy.live = false;
		enemy.speed = 2;
		enemy.boundx = 122;
		enemy.boundy = 128;	
}
void DrawEnemy(Enemy &enemy, int maxFrame, int curFrame, int frameCount, int frameDelay, int frameWidth, int frameHeight, ALLEGRO_BITMAP *wrogowie)
{

		if (enemy.live)
			al_draw_bitmap_region(wrogowie, curFrame * frameWidth, 0, frameWidth, frameHeight, enemy.x, enemy.y, 0);
		
}
void StartEnemy(Enemy &enemy)
{
		if (!enemy.live)
		{
				enemy.live = true;
				enemy.x = width;
				enemy.y = 590;		
			

		}
}
void UpdateEnemy(Enemy &enemy, int maxFrame, int curFrame, int frameCount, int frameDelay, int frameWidth, int frameHeight, ALLEGRO_BITMAP *wrogowie, ALLEGRO_BITMAP *ataki)
{
			if (++frameCount >= frameDelay)
				if (++curFrame >= maxFrame)
				{
					curFrame = 0;
				}
			frameCount = 0;
		
			enemy.x -= enemy.speed;
			if (enemy.x <= 300)
			{
				enemy.x = 300;
				al_draw_bitmap_region(ataki, curFrame * frameWidth, 0, frameWidth, frameHeight, enemy.x, enemy.y, 0);
			}
			else
				al_draw_bitmap_region(wrogowie, curFrame * frameWidth, 0, frameWidth, frameHeight, enemy.x, enemy.y, 0);
}	

void InitTower(Tower &tower)
{
	tower.x = 50;
	tower.y = 300;
	tower.ID = TOWER;
	tower.hp = 100;
}
void DrawTower(Tower &tower, ALLEGRO_BITMAP *wieza)
{
	al_draw_bitmap(wieza, tower.x, tower.y, 0);
}
void UpdateTower(Tower &tower);