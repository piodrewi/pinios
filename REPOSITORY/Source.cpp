#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>				
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

enum KLUCZE{ LEWO, PRAWO };


int main(void)
{
	int width = 1366;
	int height = 768;
	int FPS = 60;

	const int maxFrame = 3;
	int curFrame = 0;
	int frameCount = 0;
	int frameDelay = 5;
	int frameWidth = 122;
	int frameHeight = 128;
	int x = 1366;
	bool klucze[2] = { false, false };
	bool done = false;

	bool lewo = false;	// ZMIENNE DO ZMIANY BITMAPY
	bool prawo = false;		// LEWO<->PRAWO
	
	int pozycja_myszki_X = 0;
	int pozycja_myszki_y = 0;

	int pos_x = 115; // POZYCJA CZARODZIEJA + OGRANICZENIE RUCHU
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_BITMAP *czarodziej = NULL;
	ALLEGRO_BITMAP *czarodziej2 = NULL;
	ALLEGRO_BITMAP *mapa = NULL;
	ALLEGRO_BITMAP *wieza = NULL;
	ALLEGRO_BITMAP *czarodziej_strzal1 = NULL;
	ALLEGRO_BITMAP *czarodziej_strzal2 = NULL;
	ALLEGRO_BITMAP *wrogowie = NULL;
	ALLEGRO_BITMAP *ataki = NULL;
	ALLEGRO_TIMER *timer = NULL;


	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL,
			"FAILURE TO INITIATE ALLEGRO!", NULL, NULL);
		return -1;
	}

	display = al_create_display(width, height);

	if (!display)
	{
		al_show_native_message_box(NULL, NULL, NULL,
			"FAILURE TO INITIATE ALLEGRO!", NULL, NULL);
		return -1;
	}
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_mouse();
	

	mapa = al_load_bitmap("mapa.png");
	czarodziej = al_load_bitmap("2.png");
	czarodziej2 = al_load_bitmap("2a.png");
	czarodziej_strzal1 = al_load_bitmap("strzelajacylewo.png");
	czarodziej_strzal2 = al_load_bitmap("strzelajacyprawo.png");
	wrogowie = al_load_bitmap("wrogie.png");
	wieza = al_load_bitmap("wiezyczka2.png");
	ataki = al_load_bitmap("ataki.png");

	timer = al_create_timer(1.0 / FPS);
	
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_start_timer(timer);

	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_RIGHT:
				klucze[PRAWO] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				klucze[LEWO] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_RIGHT:
				klucze[PRAWO] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				klucze[LEWO] = false;
				break;
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			}
		}
	/*	else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			pozycja_myszki_X = ev.mouse.x;
			pozycja_myszki_y = ev.mouse.y;
		}

		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (ev.mouse.button & 1)
			{
				al_draw_bitmap(czarodziej_strzal2, pos_x, 140, 0);
				al_draw_filled_circle(500, 300, 25, al_map_rgb(255, 125, 125));
				al_flip_display();
				al_rest(1.0);
			}
		}*/// zakomentowana akcja myszki.

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}

		al_draw_bitmap(mapa, 0, 0, 0);
		al_draw_bitmap(wieza, 50, 300, 0);
		if (++frameCount >= frameDelay)
		{
			if (++curFrame >= maxFrame)
			{
				curFrame = 0;
			}
				
				frameCount = 0;
		}

		x =x-1;

		if (x <= 420 - frameWidth)
			x = width;
		al_draw_bitmap_region(wrogowie, curFrame * frameWidth, 0, frameWidth, frameHeight, x, 590, 0);

		if (pos_x < 90)
		{
			pos_x += 0;
		}
		else
		{
			pos_x -= klucze[LEWO] * 3;
		}
		if (pos_x > 120)
		{
			pos_x += 0;
		}
		else
		{
			pos_x += klucze[PRAWO] * 3;
		}

		if (klucze[LEWO])
		{
			al_draw_bitmap(czarodziej2, pos_x, 140, 0);
			lewo = true;
			prawo = false;
		}
		else if (klucze[PRAWO])
		{
			al_draw_bitmap(czarodziej, pos_x, 140, 0);
			lewo = false;
			prawo = true;
		}
		if (prawo)
		{
			al_draw_bitmap(czarodziej, pos_x, 140, 0);
		}
		else if (lewo)
		{
			al_draw_bitmap(czarodziej2, pos_x, 140, 0);
		} // PORUSZANIE CZARODZIEJA
		
		al_flip_display();
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
