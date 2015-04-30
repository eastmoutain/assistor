/*
 * window.h
 *
 *  Created on: Mar 18, 2015
 *      Author: root
 */

#ifndef INC_WINDOW_H_
#define INC_WINDOW_H_

#include <gtk/gtk.h>

enum ASSIST_STATE{
	UNLOGED_IN = 0,
	LOGED_IN,
	QUESTION_SENT,
	IN_DIALOG
};

struct pos{
	gint x;
	gint y;
};

#define MAX_QUESTION_LEN 256


struct mainwindowProp{

#define MAIN_WINDOW_HSIZE 400
#define MAIN_WINDOW_VSIZE 300
#define MAIN_WINDOW_BOARD 5

	gpointer backgroud;
	GtkWidget *window;
	gint window_border;
	struct pos window_size;
	GtkWidget *fixed;
	GtkWidget *name_hbox;
	struct pos name_hbox_pos;
	GtkWidget *password_hbox;
	struct pos password_hbox_pos;
	GtkWidget *question_hbox;
	struct pos question_hbox_pos;
	GtkWidget *scroll_window;
	GtkWidget *next_hbox;
	struct pos next_hbox_pos;

	GtkWidget *name_label;
	GtkWidget *username_entry;
	GtkWidget *password_label;
	GtkWidget *password_entry;
	GtkWidget *question_label;
	GtkWidget *question_entry;
	GtkWidget *question_text_view;
	GtkTextBuffer *question_text_view_buffer;
	struct pos question_entry_size;

	GtkWidget *changePassword_btn;
	struct pos changePassword_btn_pos;
	GtkWidget *request_btn;
	GtkWidget *cancel_btn;
};


struct changePasswordWindowProp{

#define CHANGE_PASSWORD_WINDOW_BORDER 5
#define CHANGE_PASSWORD_WINDOW_HSIZE 380
#define CHANGE_PASSWORD_WINDOW_VSIZE 250

	gint	window_border;
	gpointer background;
	GtkWidget *window;
	struct pos window_size;
	GtkWidget *fixed;

	GtkWidget *username_label;
	GtkWidget *username_entry;
	GtkWidget *username_hbox;
	struct pos username_hbox_pos;

	GtkWidget *oldPassword_label;
	GtkWidget *oldPassword_entry;
	GtkWidget *oldPassword_hbox;
	struct pos oldPassword_hbox_pos;

	GtkWidget *newPassword_label;
	GtkWidget *newPassword_entry;
	GtkWidget *newPassword_hbox;
	struct pos newPassword_hbox_pos;

	//GtkWidget *password_criterion_label;

	GtkWidget *confirmPassword_label;
	GtkWidget *confirmPassword_entry;
	GtkWidget *confirmPassword_hbox;
	struct pos confirmPassword_hbox_pos;

	GtkWidget *sure_btn;
	GtkWidget *cancel_btn;
	GtkWidget *next_hbox;
	struct pos next_hbox_pos;
};
/*
struct assistorWindowProp{
	GtkWidget *label1;
	GtkWidget *label2;
	GtkWidget *label3;
};
*/

struct dialogWindowProp{

#define DIALOG_WINDOW_HSIZE 350
#define DIALOG_WINDOW_VSIZE 200
#define DIALOG_WINDOW_BOARD 5
	gint window_border;
	GtkWidget * window;
	struct pos window_size;
	GtkWidget *fixed;
	GtkWidget *label_vbox;
	struct pos label_vbox_pos;
	GtkWidget *label1;
	struct pos label1_size;
	GtkWidget *label2;
	struct pos label2_size;
	GtkWidget *label3;
	struct pos label3_size;

	GtkWidget *refresh_btn;
	struct pos refresh_btn_size;
	GtkWidget *close_btn;
	struct pos close_btn_size;

	GtkWidget *auto_refresh_btn;
	struct pos auto_refresh_btn_size;

	GtkWidget *next_hbox;
	struct pos next_hbox_pos;

	gchar desc_id[5];
	gint timer_id;
};


struct appWindows{
	struct mainwindowProp *mw;
	struct changePasswordWindowProp* cpw;
	struct dialogWindowProp *dw;
};

//void init_windows();
struct mainwindowProp* createMainWindow(void);
struct dialogWindowProp* createDialogWindow(void);
struct changePasswordWindowProp* createChangePasswordWindow(void);
void createReloginWindow(void);

void changePasswordSure(GtkWidget *w, gpointer p);
void changePassword(GtkWidget *w, gpointer p);
void changePasswordCancel(GtkWidget *w, gpointer p);
void request(GtkWidget *w, gpointer data);
void assistDialog(GtkWidget *w, gpointer data);
void sigint_h1(int sig);
void sigint_h2(int sig);



#endif /* INC_WINDOW_H_ */
