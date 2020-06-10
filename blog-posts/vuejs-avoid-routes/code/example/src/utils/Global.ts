let isRouteChangeBlocked: boolean = false;

export function blockRouteChange(set?: boolean): boolean {
    if (arguments.length == 1) {
        isRouteChangeBlocked = !!set;
        return isRouteChangeBlocked;
    }

    return isRouteChangeBlocked;
}
