#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

int main()
{
    /*
    Return:
        0 if the dialog window was closed without activating a button.
        1 if the OK or Yes button was pressed.
        2 if the Cancel or No button was pressed.

    Flags:
        ALLEGRO_MESSAGEBOX_WARN
            The message is a warning. This may cause a different icon (or other effects).
        ALLEGRO_MESSAGEBOX_ERROR
            The message is an error.
        ALLEGRO_MESSAGEBOX_QUESTION
            The message is a question.
        ALLEGRO_MESSAGEBOX_OK_CANCEL
            Instead of the "OK" button also display a cancel button. Ignored if buttons is not NULL.
        ALLEGRO_MESSAGEBOX_YES_NO
            Instead of the "OK" button display Yes/No buttons. Ignored if buttons is not NULL.
    */

    int button = al_show_native_message_box( NULL, "Title", "Question", "Description", NULL, ALLEGRO_MESSAGEBOX_YES_NO);
    if(button == 0)
    {
        al_show_native_message_box(NULL, "Title", "", "Message box was closed", NULL, ALLEGRO_MESSAGEBOX_ERROR);
    }
    else if(button == 1)
    {
        al_show_native_message_box(NULL, "Result", "", "Yes clicked", NULL, ALLEGRO_MESSAGEBOX_WARN);
    }
    else if (button == 2)
    {
        al_show_native_message_box(NULL, "Result", "", "No clicked", NULL, ALLEGRO_MESSAGEBOX_WARN);
    }
    return 0;
}
