<template>
  <div>
    <h1 class="heading">IPK Projekt 1 - Showcase</h1>
    <h2>Computer status panel</h2>
    <div class="grid">
      <va-card class="computer-card" v-for="computer in computerAddresses" :key="computer.ip" :color="computer.color">
        <va-card-title>IP: {{computer.ip}}</va-card-title>
        <va-card-content class="card-content">
          <p>Hostname: <b>{{computer.hostname}}</b></p>
          <p>CPU Name: <b>{{computer.cpuName}}</b></p>
          <p>Load: <b>{{computer.load}}%</b></p>
        </va-card-content>
      </va-card>
    </div>
  </div>
</template>

<script>
import VaCard from 'vuestic-ui'

export default {
  name: 'App',
  components: {
    VaCard
  },
  data(){ return {
    
    computerAddresses: [
      {
        ip: "http://merlin.fit.vutbr.cz:8322",
        hostname: "merlin.fit.vutbr.cz",
        cpuName: "Intel(R) Xeon(R) Silver 4214R CPU @ 2.40GHz",
        load: 2.541,
        color: "success"
      },
      {
        ip: "http://localhost:8080",
        hostname: "LAPTOP-LBJRUVOM",
        cpuName: "Intel(R) Core(TM) i7-1065G7 CPU @ 1.30GHz",
        load: 23.636,
        color: "warning"
      },
      {
        ip: "http://192.168.1.23:8080",
        hostname: "DESKTOP-KDFCOFBH",
        cpuName: "Intel(R) Core(TM) i7-3612QM CPU @ 2.10GHz",
        load: 57.783,
        color: "danger"
      },
    ],
    computerInfo: [],

  }},
  // created(){
  //   console.log(this.computerAddresses[1])
  // },
  async created(){
    // for (var address in this.computerAddresses) {
      // let url = `${address.ip}/hostname`
      // console.log("Fetching:",url)
      // let res = await fetch(`${address.ip}/hostname`,{mode:"no-cors"})
      // let content = await res.text()
      // console.log(content)
    // }
    // this.computerAddresses.forEach(address => {
      
    //   let url = `${address.ip}/hostname`
    //   console.log("Fetching:",url)
    //   let res = await fetch(`${address.ip}/hostname`,{mode:"no-cors"})
    //   let content = await res.text()

    // });
    // console.log(content)
  },
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
.grid{
  padding: 3rem;

  display: flex; 
  justify-content: flex-start;
}
.computer-card{
  width: 40%;
  box-sizing: border-box;
  margin-right: 10px;
}
.card-content{
  text-align: left;
  color: black;
}


.heading{
  font-size: 2rem;
}
</style>
