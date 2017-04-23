#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
typedef struct event{
	long timestamp;
	int event_type;
	int pid;
	int qid;
	int mid; 
}event;

static char * event_types[40] = {
  "ENQUEUE_MSG",
  "DELAY_MSG",
  "DEQUEUE_MSG",
  "UI_UPDATE",
  "UI_INPUT",
  "ENTER_FOREGROUND",
  "EXIT_FOREGROUND",
  "SUBMIT_ASYNCTASK",
  "CONSUME_ASYNCTASK",
  "UPLOAD_TRACE",
  "UPLOAD_DONE",
  "WRITE_TRACE",
  "WRITE_DONE",
  "MSG_POLL_NATIVE",
  "MSG_POLL_DONE",
  "SWITCH_CONFIG",
  "UI_KEY_BEGIN_BATCH",
  "UI_KEY_CLEAR_META",
  "UI_KEY_COMMIT_COMPLETION",
  "UI_KEY_COMMIT_CORRECTION",
  "UI_KEY_COMMIT_TEXT",
  "UI_KEY_DELETE_TEXT",
  "UI_KEY_END_BATCH",
  "UI_KEY_FINISH_COMPOSING",
  "UI_KEY_GET_CAPS",
  "UI_KEY_PERFORM_EDITOR_ACTION",
  "UI_KEY_PERFORM_CONTEXT_MENU",
  "UI_KEY_PERFORM_PRIVATE_COMMAND",
  "UI_KEY_SET_COMPOSING_TEXT",
  "UI_KEY_SET_COMPOSING_REGION",
  "UI_KEY_SET_SELECTION",
  "UI_KEY_SEND_KEY",
  "UI_KEY_GET_TEXT_AFTER",
  "UI_KEY_GET_TEXT_BEFORE",
  "UI_KEY_GET_SELECTED_TEXT",
  "UI_KEY_GET_EXTRACTED_TEXT",
  "EVENT_OPENGL",
  "UI_INVALIDATE",
  "UI_UPDATE_VSYNC",
  "UI_UPDATE_DISPATCH"
};

#define MSG_ENQUEUE 0
#define MSG_ENQUEUE_DELAY 1
#define MSG_DEQUEUE 2
#define UI_UPDATE 3
#define UI_INPUT 4
#define ENTER_FOREGROUND 5
#define EXIT_FOREGROUND 6
#define SUBMIT_ASYNC 7
#define CONSUME_ASYNC 8
#define UPLOAD_TRACE 9
#define UPLOAD_DONE 10
#define WRITE_TRACE 11
#define WRITE_DONE 12
#define MSG_POLL_NATIVE 13
#define MSG_POLL_DONE 14
#define SWITCH_CONFIG 15
#define UI_KEY_BEGIN_BATCH 16 /* key input from IME */
#define UI_KEY_CLEAR_META 17 /* key input from IME */
#define UI_KEY_COMMIT_COMPLETION 18 /* key input from IME */
#define UI_KEY_COMMIT_CORRECTION 19 /* key input from IME */
#define UI_KEY_COMMIT_TEXT 20 /* key input from IME */
#define UI_KEY_DELETE_TEXT 21 /* key input from IME */
#define UI_KEY_END_BATCH 22 /* key input from IME */
#define UI_KEY_FINISH_COMPOSING 23 /* key input from IME */
#define UI_KEY_GET_CAPS 24 /* key input from IME */
#define UI_KEY_PERFORM_EDITOR_ACTION 25 /* key input from IME */
#define UI_KEY_PERFORM_CONTEXT_MENU 26 /* key input from IME */
#define UI_KEY_PERFORM_PRIVATE_COMMAND 27 /* key input from IME */
#define UI_KEY_SET_COMPOSING_TEXT 28 /* key input from IME */
#define UI_KEY_SET_COMPOSING_REGION 29 /* key input from IME */
#define UI_KEY_SET_SELECTION 30 /* key input from IME */
#define UI_KEY_SEND_KEY 31 /* key input from IME */
#define UI_KEY_GET_TEXT_AFTER 32 /* key input from IME */
#define UI_KEY_GET_TEXT_BEFORE 33 /* key input from IME */
#define UI_KEY_GET_SELECTED_TEXT 34 /* key input from IME */
#define UI_KEY_GET_EXTRACTED_TEXT 35 /* key input from IME */ 
#define EVENT_OPENGL 36 /* event indicating openGL usage */
#define UI_INVALIDATE 37
#define UI_UPDATE_VSYNC 38
#define UI_UPDATE_DISPATCH 39

void print_event_json(event cur, FILE* outstream)
{
	long timestamp = cur.timestamp;
	int event_type = cur.event_type;
	int pid = cur.pid;
	const char* json_start = "{\"event\":\"%s\",\"time\":{\"sec\":%d,\"usec\":%d},\"pid\":%d,\"data\":";
	const char* json_end = "}\n";
	int sec = timestamp/1e6;
	int usec = timestamp - sec*1e6;
        if (event_type < 0 || event_type > 39 || sec < 0 || usec < 0 || pid < 0)
            return;
	fprintf(outstream, json_start, event_types[event_type], sec, usec, pid);
        switch(event_type)
	{
		case MSG_ENQUEUE:
		case MSG_DEQUEUE:
		{
			int mid = cur.mid;
			int qid = cur.qid;
			fprintf(outstream, "{\"message_id\":%d,\"queue_id\":%d}",mid, qid);
			break;
		}
		case MSG_ENQUEUE_DELAY:
		{
			int delay_millis = cur.qid;
			fprintf(outstream, "{\"delay\":%d}", delay_millis);
			break;
		}
		case SUBMIT_ASYNC:
		case CONSUME_ASYNC:
		{
			int runnable = cur.qid;
			fprintf(outstream, "{\"runnable\":%d}", runnable);
			break;
		}
		case SWITCH_CONFIG:
		{
			int config = cur.qid;
			const char * core = (config & 0x02)?"single":"duo";
			const char *  DVFS = (config & 0x01)?"off":"on";
			fprintf(outstream, "{\"core\":%s, \"DVFS\":%s}", core, DVFS);
			break;
		}
		case UI_UPDATE:
		case UI_INPUT:
                case UI_INVALIDATE:
                case UI_UPDATE_VSYNC:
                case UI_UPDATE_DISPATCH:
                case UI_KEY_BEGIN_BATCH:
                case UI_KEY_CLEAR_META:
                case UI_KEY_COMMIT_COMPLETION:
                case UI_KEY_COMMIT_CORRECTION:
                case UI_KEY_COMMIT_TEXT:
                case UI_KEY_DELETE_TEXT:
                case UI_KEY_END_BATCH:
                case UI_KEY_FINISH_COMPOSING:
                case UI_KEY_GET_CAPS:
                case UI_KEY_PERFORM_EDITOR_ACTION:
                case UI_KEY_PERFORM_CONTEXT_MENU:
                case UI_KEY_PERFORM_PRIVATE_COMMAND:
                case UI_KEY_SET_COMPOSING_TEXT:
                case UI_KEY_SET_COMPOSING_REGION:
                case UI_KEY_SET_SELECTION:
                case UI_KEY_SEND_KEY:
                case UI_KEY_GET_TEXT_AFTER:
                case UI_KEY_GET_TEXT_BEFORE:
                case UI_KEY_GET_SELECTED_TEXT:
                case UI_KEY_GET_EXTRACTED_TEXT:
                case EVENT_OPENGL:
		case ENTER_FOREGROUND:
		case EXIT_FOREGROUND:
		case UPLOAD_TRACE:
		case UPLOAD_DONE:
		case WRITE_TRACE:
		case WRITE_DONE:
		case MSG_POLL_NATIVE:
		case MSG_POLL_DONE:
			fprintf(outstream, "null");
		default:
			assert((event_type >=0) ||(event_type < 16));
	}	
	fprintf(outstream, json_end);
}
int main(){
	FILE* instream = stdin;
	FILE* outstream = stdout;
	
	event tmp_event;

	while(!feof(instream))
	{
		fread(&tmp_event, sizeof(event), 1, instream);
		print_event_json(tmp_event, outstream);
		//printf("%ld\n", tmp_event.timestamp);
	}
	return 0;
}
