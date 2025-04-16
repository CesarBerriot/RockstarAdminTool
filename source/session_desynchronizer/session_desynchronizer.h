#include <stdbool.h>

bool @is_desynchronizing_session(void);
void @desynchronize_session(void);
void @update_session_desynchronizer(void);
int @compute_session_desynchronizer_seconds_left(void);
