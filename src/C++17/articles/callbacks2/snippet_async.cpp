// A generic, long lasting function.
void processEntity(Entity& thing) {
    // Do lots of computation on "thing".
    thing.release();
}

// A callback processing the entity inline.
void onNewEntity(Entity& thing){
    processEntity(thing);
}

// A callback processing the entity asynchronously.
void onNewEntity(Entity& thing){
    std::async(std::launch::async, processEntity, thing);
}
