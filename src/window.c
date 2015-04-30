/*
 * window.c
 *
 *  Created on: Mar 18, 2015
 *      Author: root
 */

#include "window.h"
#include <stdlib.h>
#include "ccurl.h"
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <pthread.h>
#include "secretary.h"

extern struct appWindows app;

static void cpw_entry_insert_text_cb(GtkEditable *edit, gchar *text, gint len, gpointer position, gpointer data);
static void cpw_entry_delete_text_cb(GtkEditable *edit, gint start, gint end, gpointer data);
static void mw_entry_insert_text_cb(GtkEditable *edit, gchar *text, gint len, gpointer position, gpointer data);
static void mw_entry_delete_text_cb(GtkEditable *edit, gint start, gint end, gpointer data);
static void mw_quesiton_insert_cb(GtkTextView *text_view,
							gchar *string,
							gpointer user_data);

static void mw_question_text_buffer_insert(GtkTextBuffer *textbuffer,
											GtkTextIter *location,
											gchar *text,
											gint len,
											gpointer user_data);

static char *get_text_from_text_view(GtkWidget *tv);
static void deleteAssist(GtkWidget *w, gpointer data);
static void quitApp(GtkWidget *w, gpointer data);

static void auto_refres_btn_toggled(GtkWidget *w, gpointer data);
static gint refresh_hander(gpointer data);

GdkPixbuf *create_pixbuf(const gchar *filename);

extern int assist_state;

/*	Signal "Ctrl + c" handler. */
void sigint_h1(int sig)
{
	if(assist_state > LOGED_IN)
		deleteAssist(NULL, &app);
	else
		quitApp(NULL, &app);
	signal(SIGINT, SIG_DFL);
	return;
}

void sigint_h2(int sig)
{
	gtk_main_quit();
	signal(SIGINT, SIG_DFL);
	return;
}


struct mainwindowProp* createMainWindow()
{
	struct mainwindowProp *p = NULL;
	p = malloc(sizeof(struct mainwindowProp));
	if(NULL == p){
		g_print("malloc mainwindowProp failed\n");
		return NULL;
	}

	p->window_border = MAIN_WINDOW_BOARD;
	p->window_size.x = MAIN_WINDOW_HSIZE;
	p->window_size.y = MAIN_WINDOW_VSIZE;
	p->name_hbox_pos.x = (gint)(1.0/12 * (p->window_size.x));
	p->name_hbox_pos.y = (gint)(1.0/6* (p->window_size.y));
	p->password_hbox_pos.x = (gint)((1.0/12) * (p->window_size.x));
	p->password_hbox_pos.y = (gint)((11.0/40) * (p->window_size.y));
	p->question_hbox_pos.x = (gint)((1.0/12) * (p->window_size.x));
	p->question_hbox_pos.y = (gint)((8.0/20) * (p->window_size.y));
	p->next_hbox_pos.x = (gint)((3.0/5) * (p->window_size.x));
	p->next_hbox_pos.y = (gint)((7.0/8) * (p->window_size.y));
	p->changePassword_btn_pos.x = (gint)((1.0/12) * (p->window_size.x));
	p->changePassword_btn_pos.y = (gint)((6.0/8) * (p->window_size.y));

	g_print("window (%d,%d)\n",p->window_size.x, p->window_size.y);
	g_print("name_hbox (%d,%d)\n",p->name_hbox_pos.x, p->name_hbox_pos.y);
	g_print("password_hbox (%d,%d)\n",p->password_hbox_pos.x, p->password_hbox_pos.y);
	g_print("question_hbox (%d,%d)\n",p->question_hbox_pos.x, p->question_hbox_pos.y);
	g_print("next_hbox (%d,%d)\n",p->next_hbox_pos.x, p->next_hbox_pos.y);

	p->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(p->window), "远程协助");
	gtk_widget_set_size_request(GTK_WIDGET(p->window), p->window_size.x, p->window_size.y);
	gtk_window_set_resizable(GTK_WINDOW(p->window), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(p->window), p->window_border);
	gtk_window_set_position(GTK_WINDOW(p->window), GTK_WIN_POS_CENTER);
	//gtk_window_set_icon(GTK_WINDOW(p->window), create_pixbuf("../images/neokylin-logo.jpg"));
	p->fixed = gtk_fixed_new();

	gtk_container_add(GTK_CONTAINER(p->window), p->fixed);

	p->name_label = gtk_label_new("用户名：");
	// GTK_JUSTIFY_LFEF, RIGHT, CENTER(the default one), FILL
	gtk_label_set_justify(GTK_LABEL(p->name_label), GTK_JUSTIFY_LEFT);
	p->username_entry = gtk_entry_new();
	gtk_widget_set_size_request(GTK_WIDGET(p->name_label), (gint)80, (gint)25);
	gtk_widget_set_size_request(GTK_WIDGET(p->username_entry), (gint)150, (gint)25);
	gtk_entry_set_max_length(GTK_ENTRY(p->username_entry), 15);
	p->name_hbox = gtk_hbox_new(FALSE, 10);
	gtk_box_pack_start(GTK_BOX(p->name_hbox), p->name_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(p->name_hbox), p->username_entry, FALSE, FALSE, 0);
	gtk_fixed_put(GTK_FIXED(p->fixed), p->name_hbox, p->name_hbox_pos.x, p->name_hbox_pos.y);
	//gtk_fixed_put(GTK_FIXED(p->fixed), p->name_hbox, 100, 80);


	p->password_label = gtk_label_new("密码：");
	gtk_label_set_justify(GTK_LABEL(p->password_label), GTK_JUSTIFY_LEFT);
	p->password_entry = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(p->password_entry), FALSE);
	p->password_hbox = gtk_hbox_new(FALSE, 10);
	gtk_widget_set_size_request(GTK_WIDGET(p->password_label), (gint)80, (gint)25);
	gtk_widget_set_size_request(GTK_WIDGET(p->password_entry), (gint)150, (gint)25);
	gtk_entry_set_max_length(GTK_ENTRY(p->password_entry), 15);
	gtk_box_pack_start(GTK_BOX(p->password_hbox), p->password_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(p->password_hbox), p->password_entry, FALSE, FALSE, 0);
	gtk_fixed_put(GTK_FIXED(p->fixed), p->password_hbox, p->password_hbox_pos.x, p->password_hbox_pos.y);

	p->question_label = gtk_label_new("问题描述：");
	gtk_label_set_justify(GTK_LABEL(p->question_label), GTK_JUSTIFY_LEFT);
	p->question_text_view = gtk_text_view_new();
	// word wrap mode
	gtk_text_view_set_wrap_mode((p->question_text_view), GTK_WRAP_WORD_CHAR);
	p->question_text_view_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(p->question_text_view));
	p->question_entry_size.x = 200;
	p->question_entry_size.y = 70;
	gtk_widget_set_size_request(GTK_WIDGET(p->question_label), (gint)80, (gint)25);
	/*gtk_widget_set_size_request(GTK_WIDGET(p->question_text_view),
			200, 100); */
	p->scroll_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_size_request(GTK_WIDGET(p->scroll_window),
											220, 100);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(p->scroll_window),
									GTK_POLICY_AUTOMATIC,
									GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(p->scroll_window),
											p->question_text_view);
	p->question_hbox = gtk_hbox_new(FALSE, 10);
	gtk_box_pack_start(GTK_BOX(p->question_hbox), p->question_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(p->question_hbox), p->scroll_window, FALSE, FALSE, 0);
	gtk_fixed_put(GTK_FIXED(p->fixed), p->question_hbox, p->question_hbox_pos.x, p->question_hbox_pos.y);

	p->changePassword_btn = gtk_button_new_with_label("更改密码");
	gtk_fixed_put(GTK_FIXED(p->fixed), p->changePassword_btn, p->changePassword_btn_pos.x, p->changePassword_btn_pos.y);

	p->request_btn = gtk_button_new_with_label("请求");
	gtk_widget_set_sensitive(GTK_WIDGET(p->request_btn), FALSE);
	p->cancel_btn = gtk_button_new_with_label("取消");
	gtk_widget_set_size_request(GTK_WIDGET(p->request_btn), (gint)60, (gint)(25));
	gtk_widget_set_size_request(GTK_WIDGET(p->cancel_btn), (gint)60, (gint)(25));
	p->next_hbox = gtk_hbox_new(FALSE, 10);
	gtk_box_pack_start(GTK_BOX(p->next_hbox), p->request_btn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(p->next_hbox), p->cancel_btn, FALSE, FALSE, 0);
	gtk_fixed_put(GTK_FIXED(p->fixed), p->next_hbox, p->next_hbox_pos.x, p->next_hbox_pos.y);



	g_signal_connect(G_OBJECT(p->window), "destroy", G_CALLBACK(quitApp), &app);
	g_signal_connect(G_OBJECT(p->cancel_btn), "clicked", G_CALLBACK(quitApp), &app);
	g_signal_connect(G_OBJECT(p->changePassword_btn), "clicked", G_CALLBACK(changePassword), &app);
	g_signal_connect(G_OBJECT(p->request_btn), "clicked", G_CALLBACK(request), &app);
	g_signal_connect_after(G_OBJECT(p->username_entry), "insert-text", G_CALLBACK(mw_entry_insert_text_cb), &app);
	g_signal_connect_after(G_OBJECT(p->username_entry), "delete-text", G_CALLBACK(mw_entry_delete_text_cb), &app);
	g_signal_connect_after(G_OBJECT(p->password_entry), "insert-text", G_CALLBACK(mw_entry_insert_text_cb), &app);
	g_signal_connect_after(G_OBJECT(p->password_entry), "delete-text", G_CALLBACK(mw_entry_delete_text_cb), &app);
	g_signal_connect(G_OBJECT(p->question_text_view),
										"insert-at-cursor",
										G_CALLBACK(mw_quesiton_insert_cb),
										&app);
	g_signal_connect(G_OBJECT(p->question_text_view),
										"backspace",
										G_CALLBACK(mw_quesiton_insert_cb),
										&app);
	g_signal_connect_after(G_OBJECT(p->question_text_view_buffer),
								"insert-text",
								G_CALLBACK(mw_question_text_buffer_insert),
								&app);
	gtk_widget_show_all(p->window);

	return p;
}

struct changePasswordWindowProp* createChangePasswordWindow(void)
{
	struct changePasswordWindowProp *p = malloc(sizeof(struct changePasswordWindowProp));
	if(NULL == p){
		g_print("malloc changepasswordWindowProp failed");
		return NULL;
	}

	p->window_size.x = CHANGE_PASSWORD_WINDOW_HSIZE;
	p->window_size.y = CHANGE_PASSWORD_WINDOW_VSIZE;
	p->window_border = CHANGE_PASSWORD_WINDOW_BORDER;
	p->username_hbox_pos.x = (gint)((1.0/12) * p->window_size.x);
	p->username_hbox_pos.y = (gint)((1.0/5) * p->window_size.y);
	p->oldPassword_hbox_pos.x = (gint)((1.0/12) * p->window_size.x);
	p->oldPassword_hbox_pos.y = (gint)((19.0/60) * p->window_size.y);
	p->newPassword_hbox_pos.x = (gint)((1.0/12) * p->window_size.x);
	p->newPassword_hbox_pos.y = (gint)((26.0/60) * p->window_size.y);
	p->confirmPassword_hbox_pos.x = (gint)((1.0/12) * p->window_size.x);
	p->confirmPassword_hbox_pos.y = (gint)((33.0/60) * p->window_size.y);
	p->next_hbox_pos.x = (gint)((1.0/2) * p->window_size.x);
	p->next_hbox_pos.y = (gint)((3.0/4) * p->window_size.y);

	g_print("******************* changePasswordWindowProp **************************\n");
	g_print("window (%d,%d)\n",p->window_size.x, p->window_size.y);
	g_print("username_hbox (%d,%d)\n",p->username_hbox_pos.x, p->username_hbox_pos.y);
	g_print("password_hbox (%d,%d)\n",p->oldPassword_hbox_pos.x, p->oldPassword_hbox_pos.y);
	g_print("confirmPassword_hbox_hbox (%d,%d)\n",p->confirmPassword_hbox_pos.x, p->confirmPassword_hbox_pos.y);
	g_print("next_hbox (%d,%d)\n",p->next_hbox_pos.x, p->next_hbox_pos.y);

	p->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(p->window), "修改密码");
	gtk_widget_set_size_request(GTK_WIDGET(p->window), p->window_size.x, p->window_size.y);
	gtk_window_set_resizable(GTK_WINDOW(p->window), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(p->window), p->window_border);
	gtk_window_set_position(GTK_WINDOW(p->window), GTK_WIN_POS_CENTER);
	gtk_window_set_decorated(GTK_WINDOW(p->window), TRUE);
	p->fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(p->window), p->fixed);

	p->username_label = gtk_label_new("用户名：");
	gtk_widget_set_size_request(GTK_WIDGET(p->username_label), (gint)100, (gint)25);
	p->username_entry = gtk_entry_new();
	gtk_widget_set_size_request(GTK_WIDGET(p->username_entry), (gint)150, (gint)25);
	gtk_entry_set_max_length(GTK_ENTRY(p->username_entry), 15);
	gtk_entry_set_width_chars(GTK_ENTRY(p->username_entry), 10);
	p->username_hbox = gtk_hbox_new(FALSE, 10);
	gtk_box_pack_start(GTK_BOX(p->username_hbox), p->username_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(p->username_hbox), p->username_entry, FALSE, FALSE, 0);
	gtk_fixed_put(GTK_FIXED(p->fixed), p->username_hbox, p->username_hbox_pos.x, p->username_hbox_pos.y);

	p->oldPassword_label = gtk_label_new("密码：");
	gtk_widget_set_size_request(GTK_WIDGET(p->oldPassword_label), (gint)100, (gint)25);
	p->oldPassword_entry = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(p->oldPassword_entry), 15);
	gtk_widget_set_size_request(GTK_WIDGET(p->oldPassword_entry), (gint)150, (gint)25);
	gtk_entry_set_visibility(GTK_ENTRY(p->oldPassword_entry), FALSE);
	//p->password_criterion_label = gtk_label_new("8 characters");
	//gtk_widget_set_size_request(GTK_WIDGET(p->password_criterion_label), (gint)100, (gint)25);
	p->oldPassword_hbox = gtk_hbox_new(FALSE, 10);
	gtk_box_pack_start(GTK_BOX(p->oldPassword_hbox), p->oldPassword_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(p->oldPassword_hbox), p->oldPassword_entry, FALSE, FALSE, 0);
	//gtk_box_pack_start(GTK_BOX(p->oldPassword_hbox), p->password_criterion_label, FALSE, FALSE, 0);
	gtk_fixed_put(GTK_FIXED(p->fixed), p->oldPassword_hbox, p->oldPassword_hbox_pos.x, p->oldPassword_hbox_pos.y);

	p->newPassword_label = gtk_label_new("新密码：");
	gtk_widget_set_size_request(GTK_WIDGET(p->newPassword_label), (gint)100, (gint)25);
	p->newPassword_entry = gtk_entry_new();
	gtk_widget_set_size_request(GTK_WIDGET(p->newPassword_entry), (gint)150, (gint)25);
	gtk_entry_set_max_length(GTK_ENTRY(p->newPassword_entry), 15);
	gtk_entry_set_visibility(GTK_ENTRY(p->newPassword_entry), FALSE);
	//p->password_criterion_label = gtk_label_new("8 characters");
	//gtk_widget_set_size_request(GTK_WIDGET(p->password_criterion_label), (gint)80, (gint)25);
	p->newPassword_hbox = gtk_hbox_new(FALSE, 10);
	gtk_box_pack_start(GTK_BOX(p->newPassword_hbox), p->newPassword_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(p->newPassword_hbox), p->newPassword_entry, FALSE, FALSE, 0);
	//gtk_box_pack_start(GTK_BOX(p->newPassword_hbox), p->password_criterion_label, FALSE, FALSE, 0);
	gtk_fixed_put(GTK_FIXED(p->fixed), p->newPassword_hbox, p->newPassword_hbox_pos.x, p->newPassword_hbox_pos.y);

	p->confirmPassword_label = gtk_label_new("确认密码：");
	gtk_widget_set_size_request(GTK_WIDGET(p->confirmPassword_label), (gint)100, (gint)25);
	p->confirmPassword_entry = gtk_entry_new();
	gtk_widget_set_size_request(GTK_WIDGET(p->confirmPassword_entry), (gint)150, (gint)25);
	gtk_entry_set_max_length(GTK_ENTRY(p->confirmPassword_entry), 15);
	gtk_entry_set_visibility(GTK_ENTRY(p->confirmPassword_entry), FALSE);
	p->confirmPassword_hbox = gtk_hbox_new(FALSE, 10);
	gtk_box_pack_start(GTK_BOX(p->confirmPassword_hbox), p->confirmPassword_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(p->confirmPassword_hbox), p->confirmPassword_entry, FALSE, FALSE, 0);
	gtk_fixed_put(GTK_FIXED(p->fixed), p->confirmPassword_hbox, p->confirmPassword_hbox_pos.x, p->confirmPassword_hbox_pos.y);

	p->sure_btn = gtk_button_new_with_label("确认更改");
	gtk_widget_set_sensitive(p->sure_btn, FALSE);
	gtk_widget_set_size_request(GTK_WIDGET(p->sure_btn), (gint)65, (gint)25);
	p->cancel_btn = gtk_button_new_with_label("取消");
	gtk_widget_set_size_request(GTK_WIDGET(p->cancel_btn), (gint)65, (gint)25);
	p->next_hbox = gtk_hbox_new(FALSE, 10);
	gtk_box_pack_start(GTK_BOX(p->next_hbox), p->sure_btn, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(p->next_hbox), p->cancel_btn, FALSE, FALSE, 0);
	gtk_fixed_put(GTK_FIXED(p->fixed), p->next_hbox, p->next_hbox_pos.x, p->next_hbox_pos.y);

	g_signal_connect(G_OBJECT(p->window), "destroy", G_CALLBACK(quitApp), &app);
	g_signal_connect(G_OBJECT(p->sure_btn), "clicked", G_CALLBACK(changePasswordSure), (gpointer)p);
	g_signal_connect(G_OBJECT(p->cancel_btn), "clicked", G_CALLBACK(changePasswordCancel), &app);
	g_signal_connect_after(G_OBJECT(p->username_entry), "insert-text", G_CALLBACK(cpw_entry_insert_text_cb), &app);
	g_signal_connect_after(G_OBJECT(p->username_entry), "delete-text", G_CALLBACK(cpw_entry_delete_text_cb), &app);
	g_signal_connect_after(G_OBJECT(p->oldPassword_entry), "insert-text", G_CALLBACK(cpw_entry_insert_text_cb), &app);
	g_signal_connect_after(G_OBJECT(p->oldPassword_entry), "delete-text", G_CALLBACK(cpw_entry_delete_text_cb), &app);
	g_signal_connect_after(G_OBJECT(p->newPassword_entry), "insert-text", G_CALLBACK(cpw_entry_insert_text_cb), &app);
	g_signal_connect_after(G_OBJECT(p->newPassword_entry), "delete-text", G_CALLBACK(cpw_entry_delete_text_cb), &app);
	g_signal_connect_after(G_OBJECT(p->confirmPassword_entry), "insert-text", G_CALLBACK(cpw_entry_insert_text_cb), &app);
	g_signal_connect_after(G_OBJECT(p->confirmPassword_entry), "delete-text", G_CALLBACK(cpw_entry_delete_text_cb), &app);

	//gtk_widget_show_all(p->window);
	return p;
}

struct dialogWindowProp *createDialogWindow(void)
{
	struct dialogWindowProp *p = malloc(sizeof(struct dialogWindowProp));

	if (!p)
		return NULL;
	else
		memset(p, 0, sizeof(p));

	p->window_size.x = DIALOG_WINDOW_HSIZE;
	p->window_size.y = DIALOG_WINDOW_VSIZE;
	p->window_border = DIALOG_WINDOW_BOARD;
	p->label_vbox_pos.x = (gint)((1.0/15) * p->window_size.x);
	p->label_vbox_pos.y = (gint)((1.0/4) * p->window_size.y);
	p->label1_size.x = (gint)((8.5/10) * p->window_size.x);
	p->label1_size.y = (gint)((2.0/15) * p->window_size.y);
	p->label2_size.x = (gint)((8.5/10) * p->window_size.x);
	p->label2_size.y = (gint)((2.0/15) * p->window_size.y);
	p->label3_size.x = (gint)((8.5/10) * p->window_size.x);
	p->label3_size.y = (gint)((2.0/15) * p->window_size.y);
	p->close_btn_size.x = 60;
	p->close_btn_size.y = 25;
	p->refresh_btn_size.x = 60;
	p->refresh_btn_size.y = 25;
	//p->auto_refresh_btn_size.x = 75;
	//p->auto_refresh_btn_size.y = 25;
	p->next_hbox_pos.x = (gint) ((1.0/2 * p->window_size.x));
	p->next_hbox_pos.y = (gint) ((37.0/45 * p->window_size.y));

	memset(p->desc_id,'\0', sizeof(p->desc_id));

	p->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(p->window), "远程协助");
	gtk_widget_set_size_request(GTK_WIDGET(p->window), p->window_size.x, p->window_size.y);
	gtk_window_set_resizable(GTK_WINDOW(p->window), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(p->window), p->window_border);
	gtk_window_set_position(GTK_WINDOW(p->window), GTK_WIN_POS_CENTER);
	//gtk_window_set_decorated(GTK_WINDOW(p->window), TRUE);
	p->fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(p->window), p->fixed);

	p->label1 = gtk_label_new("");
	gtk_widget_set_size_request(GTK_WIDGET(p->label1), (gint)p->label1_size.x, (gint)p->label1_size.y);
	p->label2 = gtk_label_new("");
	gtk_widget_set_size_request(GTK_WIDGET(p->label1), (gint)p->label2_size.x, (gint)p->label2_size.y);
	p->label3 = gtk_label_new("");
	gtk_widget_set_size_request(GTK_WIDGET(p->label3), (gint)p->label3_size.x, (gint)p->label3_size.y);

	p->label_vbox = gtk_vbox_new(FALSE, 3);
	gtk_box_pack_start(GTK_BOX(p->label_vbox), p->label1, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(p->label_vbox), p->label2, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(p->label_vbox), p->label3, FALSE, FALSE, 3);
	gtk_fixed_put(GTK_FIXED(p->fixed),
							p->label_vbox,
							p->label_vbox_pos.x,
							p->label_vbox_pos.y);

	p->refresh_btn = gtk_button_new_with_label("刷新");
	gtk_widget_set_size_request(GTK_WIDGET(p->refresh_btn),
								(gint)p->refresh_btn_size.x,
								(gint)p->refresh_btn_size.y);

	p->auto_refresh_btn = gtk_check_button_new_with_label("自动刷新");
	gtk_widget_set_size_request(GTK_WIDGET(p->auto_refresh_btn),
											p->auto_refresh_btn_size.x,
											p->auto_refresh_btn_size.y);

	p->close_btn = gtk_button_new_with_label("关闭");
	gtk_widget_set_size_request(GTK_WIDGET(p->close_btn),
								(gint)p->close_btn_size.x,
								(gint)p->close_btn_size.y);

	p->next_hbox = gtk_hbox_new(FALSE, 2);
	gtk_box_pack_start(GTK_BOX(p->next_hbox), p->refresh_btn, FALSE, FALSE, 5);
	//gtk_box_pack_start(GTK_BOX(p->next_hbox), p->auto_refresh_btn, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(p->next_hbox), p->close_btn, FALSE, FALSE, 5);

	g_signal_connect(G_OBJECT(p->window), "destroy", (gpointer) deleteAssist, &app);
	g_signal_connect(G_OBJECT(p->close_btn), "clicked", (gpointer)deleteAssist, &app);
	//g_signal_connect(G_OBJECT(p->window), "destroy", quitApp, &app);
	//(G_OBJECT(p->close_btn), "clicked", quitApp, &app);
	g_signal_connect(G_OBJECT(p->refresh_btn), "clicked", assistDialog, p);

	/*
	 g_signal_connect(G_OBJECT(p->auto_refresh_btn),
						"clicked",
						auto_refres_btn_toggled,
						p);
	*/
	gtk_fixed_put(GTK_FIXED(p->fixed),
					p->next_hbox,
					p->next_hbox_pos.x,
					p->next_hbox_pos.y);
	//gtk_widget_show_all(p->window);
	return p;
}

void createReloginWindow(void)
{
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *fixed = gtk_fixed_new();
	GtkWidget *vbox = gtk_vbox_new(FALSE, 3);
	GtkWidget *label = gtk_label_new("assistor 已经在运行!");
	GtkWidget *closeBtn = gtk_button_new_with_label("关闭");

	gtk_widget_set_size_request(GTK_WIDGET(window), 200, 150);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(window), 5);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	gtk_window_set_title(GTK_WINDOW(window), "远程协助");
	gtk_container_add(GTK_OBJECT(window), fixed);
	gtk_widget_set_size_request(GTK_WIDGET(label), (gint)160, (gint)40);

	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);
	gtk_fixed_put(GTK_FIXED(fixed), vbox, 20, 30);
	/*	set closeBtn size	*/
	gtk_widget_set_size_request(GTK_WIDGET(closeBtn), (gint)60, (gint) 30);
	gtk_fixed_put(GTK_FIXED(fixed), closeBtn, 100, 100);

	g_signal_connect(G_OBJECT(closeBtn), "clicked", gtk_main_quit, NULL);
	g_signal_connect(G_OBJECT(window), "destroy", gtk_main_quit, NULL);

	gtk_widget_show_all(window);
	return;
}

void changePasswordSure(GtkWidget *w, gpointer pointer)
{
#define GTK_BUTTONS_RETRY -20

	struct  changePasswordWindowProp *p = pointer;
	struct curl_result cr;
	char *str = NULL;
#define ERR_INFO_LEN 512
	char err_info[ERR_INFO_LEN];
	int num = 0;
	int info_warn = 0;
	GtkWidget *infoDialog;
	gint dialog_ret;
	gchar *username = gtk_entry_get_text(GTK_ENTRY(p->username_entry));
	gchar *oldpasswd = gtk_entry_get_text(GTK_ENTRY(p->oldPassword_entry));
	gchar *newpasswd = gtk_entry_get_text(GTK_ENTRY(p->newPassword_entry));
	gchar *confirmpasswd = gtk_entry_get_text(GTK_ENTRY(p->confirmPassword_entry));

	g_print("user name: %s, oldpasswd: %s, newpasswd: %s\n", username, oldpasswd, newpasswd);
	if(strcmp(newpasswd, confirmpasswd)){
		infoDialog = gtk_message_dialog_new(
										p->window,
										GTK_DIALOG_DESTROY_WITH_PARENT,
										GTK_MESSAGE_ERROR,
										GTK_BUTTONS_NONE,
										"\n两次密码输入不一致！\n"
										"请重新输入！\n"
										);
		gtk_window_set_title(GTK_WINDOW(infoDialog), "错误");
		gtk_dialog_add_buttons(
							infoDialog,
							"确认",
							GTK_BUTTONS_RETRY,
							NULL);
		goto show_dialog;
	}
	cr = curl_request(username, oldpasswd, newpasswd, CHANGE_PASSWORD);
	if (NULL != cr.res){
		g_print("%s\n", cr.res);
		if((str = strstr(cr.res, "\"action\": true"))) {
			infoDialog = gtk_message_dialog_new(
								p->window,
								GTK_DIALOG_DESTROY_WITH_PARENT,
								GTK_MESSAGE_INFO,
								GTK_BUTTONS_NONE,
								"\n更改密码成功！\n"
								"点击“确认”返回登录界面\n"
								);

			gtk_dialog_add_buttons(
					infoDialog,
					"确认",
					GTK_BUTTONS_OK,
					NULL);
			gtk_window_set_title(GTK_WINDOW(infoDialog), "提示");
			free (cr.res);
			goto show_dialog;

		} else {
			//
			if(CURLE_OK != cr.err_no) {
				// we assume @err_info is large enough to include the error info.
				g_print("CURLE_OK != cr.err_no\n");
				num = sprintf(err_info, "\n更改密码失败！\n错误: %s\n", curl_error[cr.err_no]);
				err_info[num] = '\0';
				g_print("err_info: %s\n", err_info);
			} else {
				// need to free @cr.res
				g_print("errrr: %s\n", cr.res);
				if((str = strstr(cr.res, "\"error\""))){
					if(strstr(str, "uid"))
						num = sprintf(err_info, "\n更改密码失败,用户名错误");
					if(strstr(str, "password error."))
						num = sprintf(err_info, "\n更改密码失败,原始密码错误");
					err_info[num] = '\0';
					//g_print("err_info: %s\n", err_info);
				}
				free(cr.res);
			}
				//err_info = get_request_err_info(cr.res);
			info_warn = 1;
			infoDialog = gtk_message_dialog_new(
								p->window,
								GTK_DIALOG_DESTROY_WITH_PARENT,
								GTK_MESSAGE_ERROR,
								GTK_BUTTONS_NONE,
								err_info
								);

			gtk_dialog_add_buttons(
					infoDialog,
					"重试",
					GTK_BUTTONS_RETRY,
					"取消",
					GTK_BUTTONS_CANCEL,
					NULL);
			gtk_window_set_title(GTK_WINDOW(infoDialog), "错误");
			// finally free the memory.
		}

show_dialog:
			//gtk_window_set_title(GTK_WINDOW(infoDialog), "Information");
			dialog_ret = gtk_dialog_run(GTK_DIALOG(infoDialog));
			gtk_widget_destroy(infoDialog);
			g_print("dialog_ret = %d\n", dialog_ret);
			switch(dialog_ret){
				case GTK_BUTTONS_OK:{
					g_print("buttons okay\n");
					gtk_widget_hide_all(GTK_WIDGET(app.cpw->window));
					gtk_widget_show_all(GTK_WIDGET(app.mw->window));
					break;
				}
				case GTK_BUTTONS_RETRY:
					g_print("buttons retry\n");
					//gtk_entry_set_text(GTK_ENTRY(app.cpw->username_entry), "");
					gtk_entry_set_text(GTK_ENTRY(app.cpw->oldPassword_entry), "");
					gtk_entry_set_text(GTK_ENTRY(app.cpw->newPassword_entry), "");
					gtk_entry_set_text(GTK_ENTRY(app.cpw->confirmPassword_entry), "");
					break;
				case GTK_BUTTONS_CANCEL:{
					gtk_widget_hide_all(GTK_WIDGET(app.cpw->window));
					gtk_widget_show_all(GTK_WIDGET(app.mw->window));
					g_print("buttons cancel");
					break;
				}
				default: break;
			}


	}
}

void changePassword(GtkWidget *w, gpointer p)
{
	struct appWindows *app = p;
	assist_state = UNLOGED_IN;
	gtk_widget_hide_all(GTK_WIDGET(app->mw->window));
	gtk_widget_show_all(GTK_WIDGET(app->cpw->window));

	//	clear the text of the @mw's all entry
	gtk_entry_set_text(GTK_ENTRY(app->mw->username_entry), "");
	gtk_entry_set_text(GTK_ENTRY(app->mw->password_entry), "");
	gtk_text_buffer_set_text(
			GTK_TEXT_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->mw->question_text_view))),
			"",
			0);
	//	clear the text of the @cpw's all entry
	gtk_entry_set_text(GTK_ENTRY(app->cpw->username_entry), "");
	gtk_entry_set_text(GTK_ENTRY(app->cpw->oldPassword_entry), "");
	gtk_entry_set_text(GTK_ENTRY(app->cpw->newPassword_entry), "");
	gtk_entry_set_text(GTK_ENTRY(app->cpw->confirmPassword_entry), "");

}

void changePasswordCancel(GtkWidget *w, gpointer p)
{
	struct appWindows *app = p;
	gtk_widget_hide_all(GTK_WIDGET(app->cpw->window));
	gtk_widget_show_all(GTK_WIDGET(app->mw->window));
}


void messageDialog(GtkWidget *w, gpointer data, int info_warn)
{
	GtkWidget *infoDialog = NULL;
	GtkWidget *window = GTK_WINDOW(w);
	infoDialog = gtk_message_dialog_new(
						window,
						GTK_DIALOG_DESTROY_WITH_PARENT,
						info_warn?GTK_MESSAGE_WARNING:GTK_MESSAGE_INFO,
						GTK_BUTTONS_NONE,
						data);

		gtk_dialog_add_buttons(
				infoDialog,
				"确认",
				GTK_BUTTONS_OK,
				NULL);

		gtk_window_set_title(GTK_WINDOW(infoDialog), info_warn?"警告":"提示");
		gtk_dialog_run(GTK_DIALOG(infoDialog));
		gtk_widget_destroy(infoDialog);

	return;
}
void request(GtkWidget *w, gpointer data)
{
	struct appWindows* p = (struct appWindows *)data;
	struct curl_result cr;
	char *find_str = NULL;
	char *end_str = NULL;
	char *err_info = NULL;
#define BUF_SIZE 512
	char tmp_buf[BUF_SIZE];
	int cnt = 0;
	int num = 0;
	char *desc = NULL;

	pthread_t empathy;
	pthread_attr_t attr;

	// info FALSE, WARNING TRUE
	int info_warn = FALSE;
	cr.res = NULL;
	cr.size = 0;

	gchar *username = (gchar *)gtk_entry_get_text(GTK_ENTRY(p->mw->username_entry));
	gchar *password = (gchar *)gtk_entry_get_text(GTK_ENTRY(p->mw->password_entry));

	GtkWidget *infoDialog;

	if(NULL == username){
		g_print("username NULL");
		return;
	}
	if(!strlen(username)){
		err_info = "\n用户名为空，请输入用户名！";
		goto show_message;
	}
	if(NULL == password){
		g_print("password NULL");
		return;
	}
	if(!strlen(password)){
		err_info = "\n密码为空，请输入密码！";
		goto show_message;
	}

	cr = curl_request(username, password, NULL, LOGIN);

	if(NULL == cr.res){
		return;
	}

#if 1
	g_print("%s\n", cr.res);
#endif
	memset(tmp_buf, '\0', sizeof(tmp_buf));
	if((find_str = strstr (cr.res, "\"action\": false"))){
		if((find_str = strstr(find_str, "user name or password error")))
			err_info = "\n用户名或密码错误！";
		else {
			if(get_error_info(cr.res, tmp_buf))
				err_info = tmp_buf;
		}
		free(cr.res);
		cr.res = NULL;
		cr.size = 0;
		goto show_message;
	} else if((find_str = strstr(cr.res, "\"action\": true"))){
			assist_state = LOGED_IN;
			g_print("verified ok!!!, %s\n", cr.res); //todo login verification is okay, send the request info to...
			// first step,free the string buffer.
			if(NULL != cr.res){
				free(cr.res);
				cr.res = NULL;
				cr.size = 0;
			}

			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
			pthread_create(&empathy, &attr, start_empathy, NULL);

			desc = get_text_from_text_view(p->mw->question_text_view);//todo get text from text and release the memory in time.
			cr = curl_request(username, desc, NULL, SUBMMIT_QUESTION);
			free(desc);
			/*if(NULL == cr.res || 0 == cr.size)
				return;
			*/
			g_print("submmit result:: %s, err_no: %d\n", cr.res, cr.err_no);
			if(CURLE_OK == cr.err_no){
				if(strstr(cr.res, "\"action\": false")) {
					memset(tmp_buf, '\0', sizeof(tmp_buf));
					get_error_info(cr.res, tmp_buf);
					err_info = tmp_buf;
					free(cr.res);
					cr.res = NULL;
					cr.size = 0;
					goto show_message;
				}
				else if(strstr(cr.res, "\"action\": true")){

					g_print("submmit question success, query the dialog \n");
					find_str = strstr(cr.res, "\"data\":");
					if(find_str){
						find_str = strstr(find_str, ":");
						end_str = strstr(find_str, "}");
						memset(p->dw->desc_id, '\0', sizeof(p->dw->desc_id));
						for(++find_str, cnt = 0;
								find_str && find_str != end_str && cnt < sizeof(p->dw->desc_id);
								++find_str){

							if(*find_str == ' ')
								continue;
							p->dw->desc_id[cnt++] = *find_str;
						}
						p->dw->desc_id[cnt] = '\0';
						g_print("desc_id: %s\n", p->dw->desc_id);
						assist_state = QUESTION_SENT;
					}
					gtk_widget_show_all(GTK_WIDGET(p->dw->window));
					gtk_widget_hide_all(GTK_WIDGET(p->mw->window));

					//assistDialog(NULL, username);
					assistDialog(NULL, NULL);
					g_print("show dialog window \n");
					//if(NULL != cr.res)
					free(cr.res);
					cr.res = NULL;
					cr.size = 0;
					return;
				} else {
					memset(tmp_buf, '\0', sizeof(tmp_buf));
					strcat(tmp_buf, "\n");
					strcat(tmp_buf, cr.res);
					err_info = tmp_buf;
					g_print("err_info: %s\n", err_info);
					free(cr.res);
					cr.res = NULL;
					cr.size = 0;
					goto show_message;
				}

			} else { // CURLE_OK != cr.err_no
				num = strcpy(tmp_buf, curl_error[cr.err_no]);
				tmp_buf[num] = '\0';
				err_info = tmp_buf;
				g_print("err_info: %s\n",err_info);
				// don't try to release the @cr.res's memory
				goto show_message;
			}

	} else {//
		// don't try to free @cr.res in this session

		g_print("connect failed: %s \n", cr.res);
		strcat(tmp_buf, "\n");
		if(strlen(cr.res) > BUF_SIZE-1){
			g_print("tmp_buf is too small to contain error info\n");
			return;
		}
		strcat(tmp_buf, cr.res);
		if(CURLE_OK == cr.err_no){
			free(cr.res);
		}
		err_info = tmp_buf;
		g_print("tmp_buf: %s\n", tmp_buf);
		info_warn = TRUE;
		goto show_message;
	}

show_message:
	messageDialog(p->mw->window, err_info, info_warn);
	return;

}

enum{
	USERNAME = 0,
	OLDPASSWORD,
	NEWPASSWORD,
	CONFIRMPASSWORD,
	MAX
};

static void cpw_get_entry_text_len(gchar *str[], gint str_len[], gpointer wp)
{
	struct changePasswordWindowProp *cwp = (struct changePasswordWindowProp *)wp;
	str[USERNAME] = (gchar *)gtk_entry_get_text(GTK_ENTRY(cwp->username_entry));
	str_len[USERNAME] = gtk_entry_get_text_length(GTK_ENTRY(cwp->username_entry));

	str[OLDPASSWORD] = (gchar *)gtk_entry_get_text(GTK_ENTRY(cwp->oldPassword_entry));
	str_len[OLDPASSWORD] = gtk_entry_get_text_length(GTK_ENTRY(cwp->oldPassword_entry));

	str[NEWPASSWORD] = (gchar *)gtk_entry_get_text(GTK_ENTRY(cwp->newPassword_entry));
	str_len[NEWPASSWORD] = gtk_entry_get_text_length(GTK_ENTRY(cwp->newPassword_entry));

	str[CONFIRMPASSWORD] = (gchar *)gtk_entry_get_text(GTK_ENTRY(cwp->confirmPassword_entry));
	str_len[CONFIRMPASSWORD] = gtk_entry_get_text_length(GTK_ENTRY(cwp->confirmPassword_entry));
}

static void cpw_entry_insert_text_cb(GtkEditable *edit, gchar *text, gint len, gpointer position, gpointer data)
{
	struct appWindows *p = (struct appWindows *)data;
	gchar *str[MAX];
	gint str_len[MAX];
	gint i;
	gboolean empty = FALSE;

	g_print("text: %s, len: %d \n", text, len);
	cpw_get_entry_text_len(str, str_len, p->cpw);
	for(i = 0; i < MAX; ++i){
		if(!str_len[i])
			empty = TRUE;
	}
	if(FALSE == empty)
		gtk_widget_set_sensitive(p->cpw->sure_btn, TRUE);

	if(edit == (GtkEditable *)p->cpw->username_entry){
	}

	if(edit == (GtkEditable *)p->cpw->oldPassword_entry){
		}

	if(edit == (GtkEditable *)p->cpw->newPassword_entry){
		}

	if(edit == (GtkEditable *)p->cpw->confirmPassword_entry){
		}
}
static void cpw_entry_delete_text_cb(GtkEditable *edit, gint start, gint end, gpointer data)
{
	struct appWindows *p = (struct appWindows *)data;
	gchar *str[MAX];
	gint str_len[MAX];
	gint i;
	gboolean empty = FALSE;


	cpw_get_entry_text_len(str, str_len, p->cpw);
	g_print("delete cb: ");
	for(i = 0; i < MAX; ++i){
		if(!str_len[i])
			empty = TRUE;
//		g_print("str_len[%d]: %d,\t", i, str_len[i]);
	}
//	g_print("\tstart:%d, end:%d\n", start, end);
	if(TRUE == empty)
		gtk_widget_set_sensitive(p->cpw->sure_btn, FALSE);
}

static void mw_quesiton_insert_cb(GtkTextView *text_view,
							gchar *string,
							gpointer user_data)
{
	g_print("insert %s in the question text view\n", string);
}

static void mw_question_text_buffer_insert(GtkTextBuffer *textbuffer,
											GtkTextIter *location,
											gchar *text,
											gint len,
											gpointer user_data)
{
	GtkTextIter iter;
	GtkTextBuffer *buffer;
	GtkTextMark *mark;

	buffer = textbuffer;

	mark = gtk_text_buffer_get_insert(textbuffer);
	gtk_text_buffer_get_iter_at_mark(textbuffer, &iter, mark);
	// todo: scroll the text view at the cursor.
	gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(app.mw->question_text_view), mark);

	int buffer_len = gtk_text_buffer_get_char_count(textbuffer);
	if (buffer_len >= MAX_QUESTION_LEN) {
		g_printf("too many characters in the buffer\n");
	}
	g_print("insert %s, ascii: %d\n", text, *text);
}

static void mw_get_entry_text_len(gchar *str[], gint str_len[], gpointer wp)
{
	struct mainwindowProp *mp = (struct mainwindowProp *)wp;
	str[USERNAME] = (gchar *)gtk_entry_get_text(GTK_ENTRY(mp->username_entry));
	str_len[USERNAME] = gtk_entry_get_text_length(GTK_ENTRY(mp->username_entry));

	str[OLDPASSWORD] = (gchar *)gtk_entry_get_text(GTK_ENTRY(mp->password_entry));
	str_len[OLDPASSWORD] = gtk_entry_get_text_length(GTK_ENTRY(mp->password_entry));

}


static void mw_entry_insert_text_cb(GtkEditable *edit, gchar *text, gint len, gpointer position, gpointer data)
{
	struct appWindows *p = (struct appWindows *)data;
	gchar *str[MAX];
	gint str_len[MAX];
	gint i;
	gboolean empty = FALSE;


	mw_get_entry_text_len(str, str_len, p->mw);
	g_print("insert cb: ");
	for(i = 0; i < NEWPASSWORD; ++i){
		if(!str_len[i])
			empty = TRUE;
	//		g_print("str_len[%d]: %d,\t", i, str_len[i]);
	}
	//	g_print("\tstart:%d, end:%d\n", start, end);
	if(FALSE == empty)
		gtk_widget_set_sensitive(p->mw->request_btn, TRUE);

}

static void mw_entry_delete_text_cb(GtkEditable *edit, gint start, gint end, gpointer data)
{
	struct appWindows *p = (struct appWindows *)data;
	gchar *str[MAX];
	gint str_len[MAX];
	gint i;
	gboolean empty = FALSE;

	mw_get_entry_text_len(str, str_len, p->mw);
	g_print("delete cb: ");
	for (i = 0; i < NEWPASSWORD; ++i) {
		if (!str_len[i])
			empty = TRUE;
	//		g_print("str_len[%d]: %d,\t", i, str_len[i]);
	}
	//	g_print("\tstart:%d, end:%d\n", start, end);
	if (TRUE == empty)
		gtk_widget_set_sensitive(p->mw->request_btn, FALSE);
	}

static void quitApp(GtkWidget *w, gpointer data)
{
	struct appWindows *p = (struct appWindows *)data;

	if (p->dw->timer_id)
		gtk_timeout_remove(p->dw->timer_id);

	g_print("free mainWindow\n");
	free(p->mw);
	g_print("free changePasswordWindow\n");
	free(p->cpw);
	g_print("free dialogWindow\n");
	free(p->dw);

	gtk_main_quit();

	return;
}

static void deleteAssist(GtkWidget *w, gpointer data)
{

	char tmp_buf[512];
	struct appWindows *p = (struct appWindows *)data;
	struct curl_result cr;
	gchar *username = (gchar *)gtk_entry_get_text(GTK_ENTRY(p->mw->username_entry));

	memset(tmp_buf, '\0', sizeof(tmp_buf));
	cr = curl_request(username, p->dw->desc_id, NULL, DELETE_DIALOG);
	if(cr.res == NULL)
		 return;
	if (CURLE_OK == cr.err_no) {

		if(strstr(cr.res, "\"action\": true")){
			g_print("delete ok\n");

		} else if(strstr(cr.res, "\"action\": false")){
			get_error_info(cr.res, tmp_buf);
			g_print("error info :%s\n", tmp_buf);
		}

		free(cr.res);

	} else {
		g_print("error info: %s\n", cr.res);
		//free(cr.res);
	}


	quitApp(NULL, p);
	return;
}

static char *get_text_from_text_view(GtkWidget *tv)
{
	GtkTextBuffer *buffer;
	GtkTextIter start, end;
	gchar *text;
	if(NULL == tv)
		return NULL;

	gtk_widget_set_sensitive(GTK_WIDGET(tv), FALSE);
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);
	text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
	gtk_text_buffer_set_modified(buffer, FALSE);
	gtk_widget_set_sensitive(tv, TRUE);

	g_print("text: %s\n", text);

	if(text) {
		text = strndup(text, strlen(text));  // todo remember to free the memory
		return text;
	}
	else
		return NULL;
}

void assistDialog(GtkWidget *w, gpointer data)
{
	struct curl_result cr;
	char *find_str;
	char *end_str;
	char *err_info = NULL;
	char remain[5];
	char assister[20];
	char status[5];
	static char phone[15];
	int num = 0;
	char str[512];
#define BUF_SIZE 512
	char tmp_buf[BUF_SIZE];
	int info_warn = FALSE;
	static int auto_refresh_flag = 0;
	struct dialogWindowProp *p = (struct dialogWindowProp *)data;

	/*	it's called from the previous window	*/
	if (NULL == w && NULL == data)
		auto_refresh_flag = 0;
	/*	it's called from refresh button clicked	*/
	if (p && w == p->refresh_btn) {
		g_print("called from refresh button clicked\n");
		auto_refresh_flag += 1;
		g_print("auto_refresh_flag: %d\n", auto_refresh_flag);
	}

	/* add a timer for auto refresh */
	if (auto_refresh_flag == 1) {
		auto_refresh_flag++;
		app.dw->timer_id =  gtk_timeout_add(10000, refresh_hander, app.dw);
		g_print("start timer_id : %d\n", app.dw->timer_id);
	}

	memset(&cr,'\0', sizeof(cr));
	memset(tmp_buf, '\0', sizeof(tmp_buf));

	gtk_label_set_text(GTK_LABEL(app.dw->label1), "");
	gtk_label_set_text(GTK_LABEL(app.dw->label2), "");
	gtk_label_set_text(GTK_LABEL(app.dw->label3), "");


	gchar *username = (gchar *)gtk_entry_get_text(GTK_ENTRY(app.mw->username_entry));

	cr = curl_request(username, NULL, NULL, ASSIST_DIALOG);
	if(NULL == cr.res ){
		return;
	}
	if(CURLE_OK != cr.err_no){
		g_print("err info: %s\n", cr.res);
		strcat(tmp_buf, "\n");
		strcat(tmp_buf, cr.res);
		/*num = memcpy(tmp_buf, cr.res, strlen(cr.res));
		tmp_buf[num] = '\0';
		*/
		g_print("tmp_buf :%s\n", tmp_buf);
		err_info = tmp_buf;
		info_warn = TRUE;
		//free(cr.res);
		goto show_message;
	}

	g_print("performed  assistdialog cr.res :%s\n", cr.res);
	if((find_str = strstr(cr.res, "\"action\": true"))) {
		if((find_str = strstr(find_str, "\"data\":"))) {
            find_str = strstr(find_str, ":");
		    end_str = strstr(find_str, "}");
            if(!find_str || !end_str)
                return;
            for(++find_str, num = 0; find_str != end_str && find_str; ++find_str) {
                if(*find_str == ' ')
                    continue;
                remain[num++] = *find_str;
            }
            remain[num] = '\0';
        } else {
        	//g_print("not find \"action\": true \n");
            return;
        }

		assist_state = IN_DIALOG;
		g_print("remain %s\n",  remain);
		/*	remain == -1, the technical supporter accepts our request, so get the supporter's
		 * ID info and display the info on the label. */
		if (!strcmp(remain, "-1")) {
			cr = curl_request(username, app.dw->desc_id, NULL, GET_SUPPORTER_INFO);
			if (NULL == cr.res) {
				return;
			}
			if (CURLE_OK != cr.err_no ) {
				g_print("err info: %s\n", cr.res);
				strcat(tmp_buf, "\n");
				strcat(tmp_buf, cr.res);
				g_print("tmp_buf :%s\n", tmp_buf);
				err_info = tmp_buf;
				info_warn = TRUE;
				//free(cr.res);
				goto show_message;
			} else {
				g_printf("curlll result : %s\n", cr.res);
				/* get the info about the technical supporter	*/
				if ((find_str = strstr(cr.res, "\"action\": true"))) {
					// get "status"
					memset(status, 0, sizeof(status));
					find_str = strstr(cr.res, "\"status\"");
					get_str(find_str, status);
					g_print("status: %s\n", status);

					if (!strcmp(status, "1")) {
					// get "phone"
						find_str = strstr(cr.res, "\"phone\"");
						memset(phone, 0, sizeof(phone));
						get_str(find_str, phone);
						g_print("phone: %s\n", phone);
					}
					// get "assister" name
					memset(assister, 0, sizeof(assister));
					if((find_str = strstr(find_str,"\"assister\":"))) {
						get_str(find_str, assister);
						g_print("technical supporter name: %s\n", assister);
						memset(str, '\0', sizeof(str));
						strcat(str, "您的远程协助请求已分配给技术支持：");
						strcat(str, assister);
						g_print("label1: %s\n", str);
						gtk_label_set_text(GTK_LABEL(app.dw->label1), str);
						memset(str, '\0', sizeof(str));
						strcat(str, "联系方式： ");
						strcat(str, phone);

						gtk_label_set_text(GTK_LABEL(app.dw->label2), str);

						if (!strcmp(status, "3")) {
							gtk_label_set_text(GTK_LABEL(app.dw->label3),
									"技术支持已经完成此次协助，请关闭!");
							g_print("auto_refresh_flag: %d\n", auto_refresh_flag);
							if (auto_refresh_flag) {
								auto_refresh_flag = 0;
								g_print("remove timer\n");
								g_print("end timer_id : %d\n", app.dw->timer_id);
								gtk_timeout_remove(app.dw->timer_id);
								g_print("hide refresh button\n");
								gtk_widget_set_sensitive(app.dw->refresh_btn, FALSE);
							}
						}
						else
							gtk_label_set_text(GTK_LABEL(app.dw->label3), "");
					}

				} else if ((find_str = strstr(cr.res, "\"action\": false"))) {
					get_error_info(cr.res, tmp_buf);
					free(cr.res);
					err_info = FALSE;
					goto show_message;
				} else {
					strcat(tmp_buf, "\n");
					strcat(tmp_buf, cr.res);
					//free(cr.res);
					err_info = tmp_buf;
					g_print("err info %s\n", tmp_buf);
					goto show_message;
				}

			}

		}
		/*	no technical support answer the request, so the request just queue up and waiting*/
		else {
			memset(str, '\0', sizeof(str));
			gtk_label_set_text(GTK_LABEL(app.dw->label1), "您的远程协助请求已提交给技术支持");

			strcat(str, "在您之前共有");
			strcat(str, remain);
			strcat(str, "人排队等待技术支持的远程协助");
			//gtk_label_set_text(GTK_LABEL(app.dw->label2), "在您之前共有人排队等待技术支持的远程协助");
			gtk_label_set_text(GTK_LABEL(app.dw->label2), str);
			gtk_label_set_text(GTK_LABEL(app.dw->label3), "请您耐心等待");
		}
		// todo display the result on lables;
		usleep(500000);
		return;
	} else if((find_str = strstr(cr.res, "\"action\": false"))) {
		get_error_info(cr.res, tmp_buf);
		free(cr.res);
		err_info = FALSE;
		goto show_message;
	}
	else {
		strcat(tmp_buf, "\n");
		strcat(tmp_buf, cr.res);
		free(cr.res);
		//err_info = tmp_buf;
		g_print("err info %s\n", tmp_buf);
		goto show_message;
	}
show_message:
	messageDialog(app.dw->window, err_info, info_warn);
	return;

}

static gint refresh_hander(gpointer data)
{
	g_print("refresh_hander called\n");
	assistDialog(NULL, data);
	return 1;
}

static void auto_refres_btn_toggled(GtkWidget *w, gpointer data)
{
	static gint timer_id;
	g_print("toggled\n");
	struct dialogWindowProp *dwp = (struct dialogWindowProp*)data;

	if (!dwp)
		return;

	if (GTK_TOGGLE_BUTTON(w)->active) {
		gtk_widget_set_sensitive(dwp->refresh_btn, FALSE);
		timer_id = gtk_timeout_add(3000, refresh_hander, NULL);
	} else {
		gtk_timeout_remove(timer_id);
		gtk_widget_set_sensitive(dwp->refresh_btn, TRUE);
	}
}

GdkPixbuf *create_pixbuf(const gchar *filename)
{
	GdkPixbuf *pixbuf = NULL;
	GError *error = NULL;

	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if (!pixbuf) {
		g_printf("get pixbuf error: %s\n", error->message);
		g_error_free(error);
	}

	return pixbuf;
}

/*
 *
 * If the result action is "true", return 0, if it has additional request data,
 * @info contains the data;
 * else, return the corresponding @action and @info
 * contains the error info.

int get_info(const struct curl_result *cr, char *info, CURL_ACTION action)
{
	int ret = -1;
	if(!cr || info)
		return ret;
	switch(action){
	case LOGIN:{

		break;
	}
	case CHANGE_PASSWORD:{
		if(CURLE_OK != cr->err_no){
			strcpy(info, curl_error[cr->err_no]);
			ret = 0;
		} else{

		}
		break;
	}
	case SUBMMIT_QUESTION:{

		break;
	}
	case ASSIST_DIALOG:{

		break;
	}
	case DELETE_DIALOG:{

		break;
	}
	default:{
		ret = -1;
		break;
	}
	}

	return ret;
}
 */
