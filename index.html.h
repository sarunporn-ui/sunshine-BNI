<!DOCTYPE html>
<html lang="th">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Sunshine Connection Model (LINE Seed Font)</title>
    
    <!-- LINE Seed Sans TH Webfont CDN -->
    <style>
        @font-face {
            font-family: 'LINE Seed Sans TH';
            src: url('https://cdn.jsdelivr.net/share/fonts/LINESeedSansTH/LINESeedSansTH-Regular.woff2') format('woff2'),
                 url('https://cdn.jsdelivr.net/share/fonts/LINESeedSansTH/LINESeedSansTH-Regular.woff') format('woff');
            font-weight: 400;
            font-style: normal;
        }
        @font-face {
            font-family: 'LINE Seed Sans TH';
            src: url('https://cdn.jsdelivr.net/share/fonts/LINESeedSansTH/LINESeedSansTH-Bold.woff2') format('woff2'),
                 url('https://cdn.jsdelivr.net/share/fonts/LINESeedSansTH/LINESeedSansTH-Bold.woff') format('woff');
            font-weight: 700;
            font-style: normal;
        }
        @font-face {
            font-family: 'LINE Seed Sans TH';
            src: url('https://cdn.jsdelivr.net/share/fonts/LINESeedSansTH/LINESeedSansTH-ExtraBold.woff2') format('woff2'),
                 url('https://cdn.jsdelivr.net/share/fonts/LINESeedSansTH/LINESeedSansTH-ExtraBold.woff') format('woff');
            font-weight: 800;
            font-style: normal;
        }

        /* บังคับให้ทั้งเว็บและอิลิเมนต์ต่างๆ ใช้ LINE Seed */
        body, input, select, button, table {
            font-family: 'LINE Seed Sans TH', sans-serif !important;
        }

        /* สไตล์สำหรับกล่องกราฟ */
        #network-graph {
            width: 100%;
            height: 500px;
            border: 1px solid #e5e7eb;
            border-radius: 0.75rem;
            background-color: #f9fafb;
        }
    </style>

    <!-- Tailwind CSS for Beautiful UI -->
    <script src="https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4"></script>
    <!-- Vue.js (Composition API) for Reactive State -->
    <script src="https://unpkg.com/vue@3/dist/vue.global.js"></script>
    <!-- Vis.js Network for Interactive Sunshine Graph -->
    <script type="text/javascript" src="https://unpkg.com/vis-network/standalone/umd/vis-network.min.js"></script>
</head>
<body class="bg-slate-50 text-slate-800 min-h-screen">

    <div id="app" class="container mx-auto px-4 py-8 max-w-6xl">
        <!-- Header -->
        <header class="text-center mb-10">
            <h1 class="text-3xl font-extrabold text-indigo-600 mb-2">🌞 Sunshine Connection Model</h1>
            <p class="text-slate-500 font-normal">บันทึกเครือข่ายคอนเนคชัน และวิเคราะห์จุดเชื่อมโยงทางธุรกิจ</p>
        </header>

        <div class="grid grid-cols-1 lg:grid-cols-3 gap-8">
            
            <!-- Left: Form Input (1 ส่วน) -->
            <div class="lg:col-span-1 bg-white p-6 rounded-2xl shadow-sm border border-slate-100 h-fit">
                <h2 class="text-xl font-bold mb-4 text-slate-700">
                    {{ isEditing ? '📝 แก้ไขข้อมูลคอนเนคชัน' : '➕ เพิ่มคอนเนคชันใหม่' }}
                </h2>
                
                <form @submit.prevent="handleSubmit" class="space-y-4">
                    <div>
                        <label class="block text-sm font-bold text-slate-600 mb-1">ชื่อ-นามสกุล / ชื่อเล่น</label>
                        <input v-model="form.name" type="text" required placeholder="เช่น คุณสมชาย"
                            class="w-full px-4 py-2 border border-slate-200 rounded-xl focus:outline-none focus:ring-2 focus:ring-indigo-500 transition">
                    </div>

                    <div>
                        <label class="block text-sm font-bold text-slate-600 mb-1">ชื่อธุรกิจ / บริษัท</label>
                        <input v-model="form.business" type="text" required placeholder="เช่น โรงพิมพ์ดิจิทัล, LINE Agency"
                            class="w-full px-4 py-2 border border-slate-200 rounded-xl focus:outline-none focus:ring-2 focus:ring-indigo-500 transition">
                    </div>

                    <div>
                        <label class="block text-sm font-bold text-slate-600 mb-1">กลุ่มเป้าหมาย (Target Group)</label>
                        <select v-model="form.target" required
                            class="w-full px-4 py-2 border border-slate-200 rounded-xl focus:outline-none focus:ring-2 focus:ring-indigo-500 bg-white transition">
                            <option value="" disabled>-- เลือกกลุ่มเป้าหมาย --</option>
                            <option v-for="tgt in targetGroups" :key="tgt" :value="tgt">{{ tgt }}</option>
                        </select>
                    </div>

                    <div class="pt-2 flex gap-2">
                        <button type="submit" 
                            class="flex-1 bg-indigo-600 hover:bg-indigo-700 text-white font-bold py-2.5 px-4 rounded-xl transition shadow-sm cursor-pointer text-center">
                            {{ isEditing ? 'อัปเดตข้อมูล' : 'บันทึกข้อมูล' }}
                        </button>
                        <button v-if="isEditing" type="button" @click="resetForm"
                            class="bg-slate-200 hover:bg-slate-300 text-slate-700 font-bold py-2.5 px-4 rounded-xl transition cursor-pointer">
                            ยกเลิก
                        </button>
                    </div>
                </form>
            </div>

            <!-- Right: Dashboard & Sunshine Model (2 ส่วน) -->
            <div class="lg:col-span-2 space-y-6">
                <!-- Sunshine Model Graph -->
                <div class="bg-white p-6 rounded-2xl shadow-sm border border-slate-100">
                    <div class="flex justify-between items-center mb-4">
                        <h2 class="text-xl font-bold text-slate-700">🌞 Sunshine Dashboard Graph</h2>
                        <span class="text-xs text-slate-400 bg-slate-100 px-2 py-1 rounded-md">ลากขยับย้ายตำแหน่ง หรือซูมเข้า-ออกได้</span>
                    </div>
                    <!-- ตรงนี้คือจุดวาด Network -->
                    <div id="network-graph"></div>
                </div>

                <!-- Data Table List -->
                <div class="bg-white p-6 rounded-2xl shadow-sm border border-slate-100">
                    <h2 class="text-xl font-bold mb-4 text-slate-700">📋 รายชื่อคอนเนคชันทั้งหมด ({{ connections.length }} คน)</h2>
                    
                    <div v-if="connections.length === 0" class="text-center py-8 text-slate-400">
                        ยังไม่มีข้อมูล กรอกข้อมูลที่ฟอร์มด้านซ้ายเพื่อเริ่มต้นสร้างโมเดลแสงอาทิตย์กันเลย!
                    </div>
                    
                    <div v-else class="overflow-x-auto">
                        <table class="w-full text-left border-collapse">
                            <thead>
                                <tr class="border-b border-slate-100 text-slate-400 text-sm">
                                    <th class="py-3 px-4 font-bold">ชื่อ</th>
                                    <th class="py-3 px-4 font-bold">ธุรกิจ</th>
                                    <th class="py-3 px-4 font-bold">กลุ่มเป้าหมาย</th>
                                    <th class="py-3 px-4 text-center font-bold">จัดการ</th>
                                </tr>
                            </thead>
                            <tbody class="divide-y divide-slate-50 text-sm">
                                <tr v-for="item in connections" :key="item.id" class="hover:bg-slate-50 transition">
                                    <td class="py-3 px-4 font-bold text-slate-700">{{ item.name }}</td>
                                    <td class="py-3 px-4 font-normal text-slate-600">{{ item.business }}</td>
                                    <td class="py-3 px-4">
                                        <span class="bg-indigo-50 text-indigo-600 px-2.5 py-1 rounded-full text-xs font-bold">
                                            {{ item.target }}
                                        </span>
                                    </td>
                                    <td class="py-3 px-4 text-center space-x-2">
                                        <button @click="editConnection(item)" class="text-amber-600 hover:text-amber-700 font-bold cursor-pointer">แก้ไข</button>
                                        <button @click="deleteConnection(item.id)" class="text-rose-600 hover:text-rose-700 font-bold cursor-pointer">ลบ</button>
                                    </td>
                                </tr>
                            </tbody>
                        </table>
                    </div>
                </div>

            </div>
        </div>
    </div>

    <!-- App Logic Scripts -->
    <script>
        const { createApp, ref, onMounted, nextTick } = Vue;

        createApp({
            setup() {
                const targetGroups = [
                    "โรงงาน", "ร้านอาหาร", "การตลาด", "HR", 
                    "จัดซื้อ", "เจ้าของธุรกิจ B2C", "ผู้รับเหมา", "นักออกแบบ"
                ];

                const connections = ref([
                    { id: 1, name: "คุณอนันต์", business: "รับทำแพ็คเกจจิ้ง", target: "โรงงาน" },
                    { id: 2, name: "คุณเบญจา", business: "ซัพพลายเออร์ผักสด", target: "ร้านอาหาร" },
                    { id: 3, name: "คุณชลทิศ", business: "เอเจนซี่โฆษณา TikTok", target: "การตลาด" },
                    { id: 4, name: "คุณดนัย", business: "ระบบสแกนนิ้วมือ", target: "HR" },
                ]);

                const form = ref({ id: null, name: '', business: '', target: '' });
                const isEditing = ref(false);

                let networkInstance = null;

                const drawGraph = () => {
                    const container = document.getElementById('network-graph');
                    const nodes = [];
                    const edges = [];

                    // 1. สร้าง Node ตรงกลางสำหรับกลุ่มเป้าหมาย (ใช้ฟอนต์ LINE Seed)
                    targetGroups.forEach((target) => {
                        const hasConnections = connections.value.some(c => c.target === target);
                        if (hasConnections) {
                            nodes.push({
                                id: `target-${target}`,
                                label: target,
                                shape: 'circle',
                                color: { background: '#4f46e5', border: '#4338ca' },
                                // ปรับค่า face ให้เรียกใช้ LINE Seed Sans TH ในตัวกราฟ
                                font: { color: '#ffffff', size: 15, face: 'LINE Seed Sans TH', bold: true },
                                margin: 15
                            });
                        }
                    });

                    // 2. สร้าง Node สำหรับบุคคล/ธุรกิจ (ใช้ฟอนต์ LINE Seed)
                    connections.value.forEach(conn => {
                        const nodeId = `user-${conn.id}`;
                        const nodeLabel = `${conn.name}\n(${conn.business})`;

                        nodes.push({
                            id: nodeId,
                            label: nodeLabel,
                            shape: 'box',
                            color: {
                                background: '#ffffff',
                                border: '#cbd5e1',
                                hover: { background: '#f8fafc', border: '#6366f1' }
                            },
                            // ปรับค่า face ให้เรียกใช้ LINE Seed Sans TH ในตัวกราฟ
                            font: { color: '#334155', size: 12, face: 'LINE Seed Sans TH' },
                            margin: 10,
                            borderRadius: 8
                        });

                        edges.push({
                            from: nodeId,
                            to: `target-${conn.target}`,
                            color: { color: '#94a3b8', highlight: '#6366f1' },
                            width: 1.5
                        });
                    });

                    const data = { nodes: nodes, edges: edges };
                    const options = {
                        physics: {
                            barnesHut: {
                                gravitationalConstant: -3000,
                                centralGravity: 0.3,
                                springLength: 95,
                                springConstant: 0.04
                            }
                        },
                        interaction: { hover: true }
                    };

                    if (networkInstance) networkInstance.destroy();
                    networkInstance = new vis.Network(container, data, options);
                };

                const handleSubmit = () => {
                    if (isEditing.value) {
                        const index = connections.value.findIndex(c => c.id === form.value.id);
                        if (index !== -1) connections.value[index] = { ...form.value };
                    } else {
                        connections.value.push({
                            id: Date.now(),
                            name: form.value.name,
                            business: form.value.business,
                            target: form.value.target
                        });
                    }
                    resetForm();
                    nextTick(() => drawGraph());
                };

                const editConnection = (item) => {
                    isEditing.value = true;
                    form.value = { ...item };
                };

                const deleteConnection = (id) => {
                    if(confirm("คุณแน่ใจใช่ไหมที่จะลบคอนเนคชันนี้?")) {
                        connections.value = connections.value.filter(c => c.id !== id);
                        drawGraph();
                        if (form.value.id === id) resetForm();
                    }
                };

                const resetForm = () => {
                    form.value = { id: null, name: '', business: '', target: '' };
                    isEditing.value = false;
                };

                onMounted(() => { drawGraph(); });

                return { targetGroups, connections, form, isEditing, handleSubmit, editConnection, deleteConnection, resetForm };
            }
        }).mount('#app');
    </script>
</body>
</html>