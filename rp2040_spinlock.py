if request.IsInit:
    SPINLOCKS = [1 for _ in range(0,32)]
elif request.IsRead:
    if SPINLOCKS[request.Offset // 4]:
        SPINLOCKS[request.Offset // 4] = 0
        request.Value = 1
    else:
        request.Value = 0
elif request.IsWrite:
    SPINLOCKS[request.Offset // 4] = 1
    request.Value = 1 << (request.Offset // 4)
