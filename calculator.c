#include <gtk/gtk.h>
#include "tinyexpr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <assert.h>
// set up GTK 'objects' to use
         GtkWidget *window;
         GtkWidget *button;
         GtkWidget *grid;
         GtkWidget *entry;
         GtkWidget *show;
         GtkEntryBuffer *buffer;
         GtkEntryBuffer *buffer1;
char history[100]="";




// Callback for the calculator buttons
static void on_button_press(GtkWidget *widget, gpointer buffer)
{
    
    const char *label = gtk_button_get_label(GTK_BUTTON(widget));
    // label is a pointer so need to evaluate the thing it's pointing
    if(*label == '=') {
        const char *exp = gtk_entry_buffer_get_text(buffer);
	gtk_entry_buffer_set_text(buffer1, history, -1);
	strcat(history,exp);
	strcat(history,"=");
        
	
        const char *c = exp;
        double r= te_interp(c, 0);
        char a[30];
        sprintf(a,"%f",r);
	char result[100];
	strcat(result,exp);
	strcat(result,"=");
	strcat(result,a);
        gtk_entry_buffer_set_text(buffer, result, -1);
	
	strcat(history,a);
	strcat(history," | ");
    } else if(*label == 'C') {
        gtk_entry_buffer_delete_text(buffer, 0, -1);
    } else if( *label== '<'){
	const char *exp = gtk_entry_buffer_get_text(buffer);
	gtk_entry_buffer_delete_text(buffer, strlen(exp)-1, -1);
    }else {
    guint len_buff = gtk_entry_buffer_get_length(buffer);
    gtk_entry_buffer_insert_text(buffer, len_buff, label, -1);
    }
}

// Optional delete signal handler
static gboolean on_delete_event(GtkWidget *widget,
        GdkEvent *event, gpointer data)
{
    /*
       Returning FALSE would send the "destroy" signal, TRUE
       would not. This is useful to display warnings abouta
       closing.
       */

    printf("delete event occurred\n");

    return FALSE;
}


int main(int argc, char *argv[])
{
 
 
 

    // necessary gtk init method
    gtk_init(&argc, &argv);

    // create a new window, set title, and connect delete/destroy signals
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    g_signal_connect(window, "delete-event", G_CALLBACK(on_delete_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // set border width
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    // create a grid and add it to the window
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // This is a buffer to hold the  text for the text view widget
    buffer = gtk_entry_buffer_new(NULL, -1);
   buffer1 = gtk_entry_buffer_new(NULL, -1);

    // an array to hold the button labels so they can be easily looped over
    char *button_labels[] = {
        "7", "8", "9", "/","<","C",
        "4", "5", "6", "*","(",")",
        "1", "2", "3", "-","^2","sqrt",
        "0", ".", "%", "+","=",
    };
    int num_labels = 23;
    int i = 0;
   // set up the text display area
     show = gtk_entry_new();
     gtk_entry_set_buffer(GTK_ENTRY(show), GTK_ENTRY_BUFFER(buffer1));
     gtk_grid_attach(GTK_GRID(grid), show, 0, 0, 6, 1);
     // set up the text display area
      entry = gtk_entry_new();
      gtk_entry_set_buffer(GTK_ENTRY(entry), GTK_ENTRY_BUFFER(buffer));
      gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 6, 1);

   

    // loop to create a button for each label in the array above, connect
    // it to the callback, and add it to the grid.
    int j;
    for(i = 12; i < num_labels+11; i++) {
        j = i / 6;

        button = gtk_button_new_with_label(button_labels[i-12]);
        g_signal_connect(button, "clicked", G_CALLBACK(on_button_press), buffer);
	gtk_grid_attach(GTK_GRID(grid), button, i - (6*j), j, 1, 1);
	
    }
    button = gtk_button_new_with_label(button_labels[i-12]);
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_press), buffer);
    gtk_grid_attach(GTK_GRID(grid), button, i - (6*j), j, 2, 1);

    // show all widgets attached to the main window
    gtk_widget_show_all(window);

    // run the GTK main loop
    gtk_main();

    return 0;
}
