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
const int NUM_ENEMYS = 5;
const int maxFrame = 8;

enum KEYS{ LEFT, RIGHT };
bool keys[2] = { FALSE, FALSE };


//PROTOTYPES
void InitPlayer(Player &player);
void DrawPlayer(Player &player, ALLEGRO_BITMAP *czarodziej, ALLEGRO_BITMAP *czarodziej2, bool postac);
void MovePlayerLeft(Player &player);
void MovePlayerRight(Player &player);

void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, Player &player);
void UpdateBullet(Bullet bullet[], int size, double  pozycja_myszki_X, double  pozycja_myszki_Y);

void CollideEnemy(Bullet bullet[], int bSize, Enemy enemy[],int cSize);

void InitEnemy(Enemy enemy[],int size);
void DrawEnemy(Enemy enemy[],int size, int maxFrame, int curFrame, int frameCount, int frameDelay, int frameWidth, int frameHeight, ALLEGRO_BITMAP *wrogowie,ALLEGRO_BITMAP *ataki);
void StartEnemy(Enemy enemy[],int size);
void UpdateEnemy(Enemy enemy[],int size, int maxFrame, int curFrame, int frameCount, int frameDelay, int frameWidth, int frameHeight, ALLEGRO_BITMAP *wrogowie, ALLEGRO_BITMAP *czarodziej2);

void InitTower(Tower &tower);
void DrawTower(Tower &tower, ALLEGRO_BITMAP *wieza);
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
	Enemy enemy[NUM_ENEMYS];

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
	InitEnemy(enemy, NUM_ENEMYS);
	InitTower(tower);

	// BITMAPS
	mapa = al_load_bitmap("mapa.png");
	czarodziej = al_load_bitmap("2.png");
	czarodziej2 = al_load_bitmap("2a.png");
	czarodziej_strzal1 = al_load_bitmap("strzelajacylewo.png");
	czarodziej_strzal2 = al_load_bitmap("strzelajacyprawo.png");
	wrogowie = al_load_bitmap("wrogie2.png");
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
			UpdateEnemy(enemy, NUM_ENEMYS, maxFrame, curFrame, frameCount, frameDelay, frameWidth, frameHeight, wrogowie, czarodziej2);
			StartEnemy(enemy, NUM_ENEMYS);
			CollideEnemy(bullets, NUM_BULLETS, enemy, NUM_ENEMYS);
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
			UpdateBullet(bullets, NUM_BULLETS, pozycja_myszki_X, pozycja_myszki_Y);
			DrawTower(tower, wieza);
			DrawEnemy(enemy, NUM_ENEMYS, maxFrame, curFrame, frameCount, frameDelay, frameWidth, frameHeight, wrogowie, ataki);
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
void DrawPlayer(Player &player, ALLEGRO_BITMAP *czarodziej, ALLEGRO_BITMAP *czarodziej2, bool postac)
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
			al_draw_filled_circle(bullet[i].x, bullet[i].y + 130, 2, al_map_rgb(255, 0, 0));
	}
}
void FireBullet(Bullet bullet[], int size, Player &player)
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
			bullet[i].angle = atan2(pozycja_myszki_X, pozycja_myszki_Y);
			bullet[i].x +=sin(bullet[i].angle) * 5;
			bullet[i].y +=cos(bullet[i].angle) * 5;
			if (bullet[i].x > width || bullet[i].y>height)
				bullet[i].live = false;
		}

	}
}

void InitEnemy(Enemy enemy[],int size)
{
	for (int i = 0; i < size; i++)
	{
		enemy[i].ID = ENEMY;
		enemy[i].live = false;
		enemy[i].speed = 3;
		enemy[i].boundx = 120;
		enemy[i].boundy = -128; 
		enemy[i].maxFrame = 143;
		enemy[i].curFrame = 0;
		enemy[i].frameCount = 0;
		enemy[i].frameDelay = 20;
		enemy[i].frameWidth = 125;
		enemy[i].frameWidth1 = 180;
		enemy[i].frameHeight = 128;
		enemy[i].frameHeight1 = 131;
		enemy[i].animationColumns = 81;
		enemy[i].animationColumns1 = 31;
		enemy[i].animationDirection = 1;

	}

}
void DrawEnemy(Enemy enemy[], int size, int maxFrame, int curFrame, int frameCount, int frameDelay, int frameWidth, int frameHeight, ALLEGRO_BITMAP *wrogowie, ALLEGRO_BITMAP *ataki)
{
	for (int i = 0; i < size; i++)
	{
		if (enemy[i].live)
		{
			int fx = (enemy[i].curFrame % enemy[i].animationColumns)*enemy[i].frameWidth;
			int fy = (enemy[i].curFrame / enemy[i].animationColumns)*enemy[i].frameHeight;
			int fxx = (enemy[i].curFrame % enemy[i].animationColumns1)*enemy[i].frameWidth1;
			int fyy = (enemy[i].curFrame / enemy[i].animationColumns1)*enemy[i].frameHeight1;
			if (enemy[i].x <= 370)
			{
			al_draw_bitmap_region(ataki, fxx, fyy, enemy[i].frameWidth1, enemy[i].frameHeight1, enemy[i].x - enemy[i].frameWidth1 / 2, enemy[i].y, 0);
			//al_draw_filled_rectangle(280, height - 60, 440, height - 180, al_map_rgba(255, 0, 255, 100));

			}
			else
			{
			al_draw_bitmap_region(wrogowie, fx, fy, enemy[i].frameWidth, enemy[i].frameHeight, enemy[i].x - enemy[i].frameWidth / 2, enemy[i].y, 0);
			al_draw_filled_rectangle(enemy[i].x - (enemy[i].boundx / 2), enemy[i].y - enemy[i].boundy, enemy[i].x + (enemy[i].boundx / 2), enemy[i].y, al_map_rgba(255, 0, 255, 100));
			}
		}

	}
}
void StartEnemy(Enemy enemy[],int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!enemy[i].live)
		{
			if (rand() % 500 == 0)
			{
				enemy[i].live = true;
				enemy[i].x = width;
				enemy[i].y = 590;
			}
		}
	}

}
void UpdateEnemy(Enemy enemy[],int size ,int maxFrame, int curFrame, int frameCount, int frameDelay, int frameWidth, int frameHeight, ALLEGRO_BITMAP *wrogowie, ALLEGRO_BITMAP *ataki)
{
	
	for (int i = 0; i < size; i++)
	{
		if (enemy[i].live)
		{
			if (enemy[i].x <= 370)
				enemy[i].x = 370;
			else
				enemy[i].x -= enemy[i].speed;

			if (++enemy[i].frameCount >= enemy[i].frameDelay)
			{
				enemy[i].curFrame += enemy[i].animationDirection;
			 	if (enemy[i].curFrame >= enemy[i].maxFrame)
					enemy[i].curFrame = 0;

				else if (enemy[i].curFrame <= 0)
					enemy[i].curFrame = enemy[i].maxFrame - 1;

				enemy[i].frameCount = 0;
			}


		}

	}
	
	
}

void CollideEnemy(Bullet bullet[], int bSize, Enemy enemy[],int cSize)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			for (int j = 0; j < cSize; j++)
			{
				if (enemy[j].live)
				{
					if (enemy[j].x <= 370)
					{
						if (bullet[i].x >280 &&
							bullet[i].x<440 &&
							bullet[i].y<height - 60 &&
							bullet[i].y >height-180)
						{
							bullet[i].live = false;
							enemy[j].live = false;
						}

					}
					else
					{
						if (bullet[i].x>enemy[j].x - (enemy[j].boundx / 2) &&
							bullet[i].x < enemy[j].x + (enemy[j].boundx/2) &&
							bullet[i].y<enemy[j].y&&
							bullet[i].y >enemy[j].y + enemy[j].boundy)
						{
							bullet[i].live = false;
							enemy[j].live = false;
						}
					}
				}
			}
		}
	}

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
	al_draw_filled_rectangle(50, height-50, 320, 400, al_map_rgba(0, 0, 0, 0));
}
void UpdateTower(Tower &tower);