#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>

int main()
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_BITMAP *bitmap = NULL;
	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL,
			"FAILURE TO INITIATE ALLEGRO!", NULL, NULL);
		return -1;
	}

	display = al_create_display(1366, 768);

	if (!display)
	{
		al_show_native_message_box(NULL, NULL, NULL,
			"FAILURE TO INITIATE ALLEGRO!", NULL, NULL);
		return -1;
	}
	al_rest(5.0);
	al_destroy_display(display);
	return 0;
}