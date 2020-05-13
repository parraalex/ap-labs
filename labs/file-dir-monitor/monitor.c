#include <stdio.h>
#include <sys/inotify.h>
#include <ftw.h>
#include <unistd.h>
#include "logger.h"
struct inotify_event *iNotifyEvent;

int main(int argc, char** argv){
	int iNotifyAPI;
    char buffer[1024 * (sizeof(struct inotify_event) + 16)];
	int dirWatched;
	char *watchSize;
    int scan;
    int flag = 1;
	if(argc == 2) {
		iNotifyAPI = inotify_init();
		if(iNotifyAPI == -1)
			errorf("iNotify API coulnt initialize, error.");
		dirWatched = inotify_add_watch(iNotifyAPI, argv[1], IN_ALL_EVENTS);
		if(dirWatched == -1)
			errorf("Problem with directory watch creation, please try again.");
        infof("Monitor ready. Starting scan...");
		while(flag == 1) {
			scan = read(iNotifyAPI, buffer, 1024*(sizeof(struct inotify_event)+16));
			for (watchSize=buffer; watchSize<buffer+scan;){
				iNotifyEvent = (struct inotify_event*) watchSize;
                if (iNotifyEvent->mask & IN_CREATE)
		            infof("An element has been created");
	            if (iNotifyEvent->mask & IN_DELETE)
		            infof("An element has been deleted");
	            if (iNotifyEvent->mask & IN_MOVE)
		            infof("An element has been renamed");
				watchSize += sizeof(struct inotify_event)+iNotifyEvent->len;
			}
		}
		inotify_rm_watch(iNotifyAPI, dirWatched);
		close(iNotifyAPI);
	}
	else
		warnf("Wrong parameters, please make sure that you only write the directory to scan");
    return 0;
}
