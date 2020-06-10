<template>
  <div class="home">
    Your text: <input v-model="input"/>
    <button @click="save"> Save </button>
  </div>
</template>

<script lang="ts">
import { Component, Prop, Vue, Watch } from 'vue-property-decorator';
import { blockRouteChange } from '@/utils/Global';

@Component
export default class Home extends Vue {
  private input: string = "";

  @Watch("input")
  private inputChange(to: string) {
    if (to && to.length > 0) {
      blockRouteChange(true);
    } else {
      blockRouteChange(false);
    }
  }

  private save() {
    blockRouteChange(false);
    alert("saved");
  }
}
</script>
