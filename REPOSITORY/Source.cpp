#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>				
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_image.h>


enum KLUCZE{ LEWO, PRAWO };
void przemieszczanie(bool klucze[2], int pos_x);


int main(void)
{
	int width = 1366;
	int height = 768;

	bool klucze[2] = { false, false };
	bool done = false;

	int pos_x = 115;
	int zmienna_czarodziej = 0;


	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_BITMAP *czarodziej = NULL;
	ALLEGRO_BITMAP *czarodziej2 = NULL;
	ALLEGRO_BITMAP *mapa = NULL;
	ALLEGRO_BITMAP *wieza = NULL;

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

	al_init_primitives_addon();
	al_install_keyboard();
	al_init_image_addon();
	mapa = al_load_bitmap("mapa.png");
	czarodziej = al_load_bitmap("2.png");
	czarodziej2 = al_load_bitmap("2a.png");
	wieza = al_load_bitmap("wiezyczka.png");

	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));

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
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
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

		al_draw_bitmap(mapa, 0, 0, 0);
		al_draw_bitmap(wieza, 50, 300, 0);

		if (klucze[LEWO])
		{
			al_draw_bitmap(czarodziej2, pos_x, 140, 0);
		}
		else
		{
			al_draw_bitmap(czarodziej, pos_x, 140, 0);
		}
		al_flip_display();
	}

	al_destroy_bitmap(mapa);
	al_destroy_bitmap(czarodziej);
	al_destroy_bitmap(czarodziej2);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);

	return 0;
}
