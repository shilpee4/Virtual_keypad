#include <gtk/gtk.h>
#include<fcntl.h>
#include<unistd.h>

// Flag to display text/asterisks
gboolean show_asterisks = FALSE;

// Global variable to store data which is to be written to the device file
gchar *global_text = "\0";

// Callback function for numeric keypad buttons
void numeric_button_clicked(GtkWidget *widget, gpointer data)
{
    // Get button label as data
    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    g_print("Numeric button clicked: %s\n", button_label);

    // Get buffer for editing
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data));

    // Append the button label to global_text (data to be sent)
    global_text = g_strconcat(global_text,button_label,NULL);
    g_print("%s",global_text);

    // Check current state
    if (show_asterisks) {
        // If asterisks, insert asterisks into the buffer
        gchar *asterisk_string = g_strdup_printf("%*s", (int)strlen(button_label), "*");
        gtk_text_buffer_insert_at_cursor(buffer, asterisk_string, -1);
        g_free(asterisk_string);
    } else {
        // If actual text, insert the button label into the buffer
        gtk_text_buffer_insert_at_cursor(buffer, button_label, -1);
    }
}

// Callback function for special buttons
void special_button_clicked(GtkWidget *widget, gpointer data)
{
    // Get button label as data (special button code)
    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));

    // Concatenate the special button code with global_text
    gchar *concatenated_string = g_strdup(button_label);
    if (g_strcmp0(global_text,"\0")!=0){
        concatenated_string = g_strconcat(button_label, global_text, NULL);
    }

    // Open the device file for writing
    int fd = open("/dev/virtual_keypad", O_WRONLY);
    if (fd < 0) {
        perror("Error opening device file");
        return;
    }

    // Write concatenated_string to the device file
    ssize_t bytes_written = write(fd, concatenated_string, strlen(concatenated_string));
    if (bytes_written < 0) {
        perror("Error writing to device file");
    }

    // Close the device file
    close(fd);

    g_free(concatenated_string);

    // Clear the text view
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data));
    gtk_text_buffer_set_text(buffer, "", -1);

    // Clear the message (global variable)
    global_text[0] = '\0';

    // Set flag to false
    show_asterisks = FALSE;

}

// Callback function for the backspace button
void on_backspace_clicked(GtkButton *button, GtkTextView *text_view) {

    g_print("Backspace button clicked \n");

    // Get buffer for editing and remove the last entry
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    GtkTextIter end_iter, start_iter;
    gtk_text_buffer_get_end_iter(buffer, &end_iter);
    gtk_text_buffer_get_iter_at_offset(buffer, &start_iter, gtk_text_iter_get_offset(&end_iter) - 1);
    gtk_text_buffer_delete(buffer, &start_iter, &end_iter);

    // Remove the last entry of the global variable
    int length = strlen(global_text);
    if (length > 0) {
        // Replace it with the null terminator
        global_text[length - 1] = '\0';
    }

    g_print("%s",global_text);

}

// Callback function for asterisk button
void asterisk_button_clicked(GtkWidget *widget, gpointer data)
{
    // Get buffer for editing
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data));

    if (show_asterisks) {

        // If showing asterisks, revert to numeric
        GtkTextIter start, end;
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);

        gtk_text_buffer_set_text(buffer, global_text, -1);
    } 
    else {

        // If showing actual text, replace with asterisks
        GtkTextIter start, end;
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        gchar *buffer_text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
        gchar *asterisk_text = g_strdup(buffer_text);
        for (int i = 0; asterisk_text[i] != '\0'; i++) {
            if (asterisk_text[i] != '\n' && asterisk_text[i] != '\r' && asterisk_text[i] != ' ') {
                asterisk_text[i] = '*';
            }
        }

        // Set the text view with asterisks
        gtk_text_buffer_set_text(buffer, asterisk_text, -1);
        g_free(asterisk_text);
        g_free(buffer_text);

    }

    // Toggle the flag
    show_asterisks = !show_asterisks;

}

int main(int argc, char *argv[])
{
    // Creation of the GUI

    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;
    GtkWidget *text_view;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Virtual Keypad");

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create the text view
    text_view = gtk_text_view_new();  
    
    // Add numeric keypad buttons
    for (int i = 1; i <= 9; ++i) {
        char button_label[2]; 
        sprintf(button_label, "%d", i);
        button = gtk_button_new_with_label(button_label);
        g_signal_connect(button, "clicked", G_CALLBACK(numeric_button_clicked), text_view);
        gtk_grid_attach(GTK_GRID(grid), button, (i - 1) % 3, (i - 1) / 3, 1, 1);
    }

    // Add text view to display text
    gtk_grid_attach(GTK_GRID(grid), text_view, 0, 3, 3, 1);

    // Add special button for Username logging
    button = gtk_button_new_with_label("USRN");
    g_signal_connect(button, "clicked", G_CALLBACK(special_button_clicked), text_view);
    gtk_grid_attach(GTK_GRID(grid), button, 4, 0, 1, 1);

    // Add special button for Password logging
    button = gtk_button_new_with_label("PASS");
    g_signal_connect(button, "clicked", G_CALLBACK(special_button_clicked), text_view);
    gtk_grid_attach(GTK_GRID(grid), button, 4, 1, 1, 1);

    // Add special button for Payment scan generation
    button = gtk_button_new_with_label("PAY!");
    g_signal_connect(button, "clicked", G_CALLBACK(special_button_clicked), text_view);
    gtk_grid_attach(GTK_GRID(grid), button, 4, 2, 1, 1);

    // Add special button for FAQ document
    button = gtk_button_new_with_label("FAQs");
    g_signal_connect(button, "clicked", G_CALLBACK(special_button_clicked), text_view);
    gtk_grid_attach(GTK_GRID(grid), button, 4, 3, 1, 1);

    // Add special button for opening Help webpage
    button = gtk_button_new_with_label("HELP");
    g_signal_connect(button, "clicked", G_CALLBACK(special_button_clicked), text_view);
    gtk_grid_attach(GTK_GRID(grid), button, 4, 4, 1, 1);

    // Add special button for Clearing text
    button = gtk_button_new_with_label("CLR!");
    g_signal_connect(button, "clicked", G_CALLBACK(special_button_clicked), text_view);
    gtk_grid_attach(GTK_GRID(grid), button, 3, 2, 1, 1);

    // Add backspace button
    GtkWidget *backspace_button = gtk_button_new_with_label("Backspace");
    g_signal_connect(backspace_button, "clicked", G_CALLBACK(on_backspace_clicked), text_view);
    gtk_grid_attach(GTK_GRID(grid), backspace_button, 3, 0, 1, 1);

    // Add asterisk button
    button = gtk_button_new_with_label("Asterisk");
    g_signal_connect(button, "clicked", G_CALLBACK(asterisk_button_clicked), text_view);
    gtk_grid_attach(GTK_GRID(grid), button, 3, 1, 1, 1);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
