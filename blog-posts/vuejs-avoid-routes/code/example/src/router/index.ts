import Vue from 'vue'
import VueRouter, { RouteConfig, RawLocation } from 'vue-router'
import Home from '../views/Home.vue'
import About from '../views/About.vue'
import { blockRouteChange } from '@/utils/Global'


Vue.use(VueRouter)

  const routes: Array<RouteConfig> = [
  {
    path: '/',
    name: 'Home',
    component: Home
  },
  {
    path: '/about',
    name: 'About',
    component: About
  }
]

const originalPush = VueRouter.prototype.push;
VueRouter.prototype.push = function(location: RawLocation) {
    if (blockRouteChange()) {
        if (confirm("There are unsaved changes, do you want to continue?")) {
            blockRouteChange(false);
            return originalPush.call(this, location) as any;
        }
        return;
    }
    return originalPush.call(this, location) as any;
};

const router = new VueRouter({
  routes
})

export default router
